/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 10:56:24 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/21 17:33:33 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(std::string const & buffer) {
    Request request(buffer);
    code = OK;
    handleResponse(request);
}

Response::~Response() {}

const char* Response::getStatusMessage(HttpStatusCode code) {
    switch (code) {
        case BadRequest: return "Bad Request";
        case Unauthorized: return "Unauthorized";
        case Forbidden: return "Forbidden";
        case NotFound: return "Not Found";
        case MethodNotAllowed: return "Method Not Allowed";
        case RequestURITooLong: return "Request-URI Too Long";
        case RequestEntityTooLarge: return "Request Entity Too Large";
        case NotImplemented: return "Not Implemented";
        default: return "OK";
    }
}

std::string Response::getResponse() {
    std::stringstream ss;
    ss << "HTTP/1.1 " << toString(code) << " " << getStatusMessage(code) << "\r\n";
    for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++) {
        ss << it->first << ": " << it->second << "\r\n";
    }
    ss << "\r\n";
    ss << body;
    return ss.str();
}

void Response::serveStaticFile(std::string const &filePath) {
    std::ifstream file(filePath);
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        body = buffer.str();
        file.close();
    } else {
        code = NotFound;
        body = getStatusMessage(code);
    }
}

Server Response::getServer(Request const & req) {
    std::vector<Server>::iterator it = Config::find(req.getHost(), req.getPort());
    if (it != Config::end()) {
        return *it;
    }
    return *(Config::begin());
}

std::string Response::getRequestedResource(std::string const & uri) {
    std::string resource = uri;
    size_t pos = resource.find('?');
    if (pos != std::string::npos) {
        resource = resource.substr(0, pos);
    }
    return resource;
}

static bool isDirectory(std::string path)
{
    struct stat stat_buf;
    if (stat(path.c_str(), &stat_buf) != 0)
        return false;

    return S_ISDIR(stat_buf.st_mode);
}

static bool isFile(std::string path)
{
    struct stat stat_buf;
    if (stat(path.c_str(), &stat_buf) != 0)
        return false;

    return S_ISREG(stat_buf.st_mode);
}

Route Response::getRoute(Server & server, Request const & req) {
    std::string resource = getRequestedResource(req.getUri());

    std::vector<Route>::iterator it = server.find(resource);
    if (it == server.end()) { // Get matched route for request
        if (isDirectory(server.getRoot() + resource)) { // Check if resource is a directory
            return Route(server.getRoot() + "/" + resource, "", Route::DIRECTORY);
        }
        else if (isFile(server.getRoot() + resource)) { // Check if resource is a file
            return Route(server.getRoot(), resource, Route::FILE);
        }
        else
            throw ServerException(NotFound);
    }
    if (!it->getRedirect().empty()) { // Check if route have redirection
        headers["Location"] = it->getRedirect();
        throw ServerException(MovedPermanently);
    }
    std::vector<std::string> methods = it->getMethods(); // Check if route have allowed methods
    if (!methods.empty() && std::find(methods.begin(), methods.end(), req.getMethod()) == methods.end()) {
        throw ServerException(MethodNotAllowed);
    }
    return *it;
}

std::string Response::tryFiles(Server const & server, Route const & route, Request const & req, std::string const & root) {
    (void) req;

    std::vector<std::string> indexes;
    // Check if route is a file if not get index
    if (route.getRouteType() == Route::FILE) {
        indexes.push_back(route.getPath());
    } else {
        indexes = route.getIndex().empty() ? (server.getIndex().empty() ? std::vector<std::string>() : server.getIndex()) : route.getIndex();
    }
    std::string filePath;
    for (std::vector<std::string>::iterator it = indexes.begin(); it != indexes.end(); it++) {
        filePath = root + "/" + *it;
        std::ifstream file(filePath);
        if (file.good()) {
            file.close();
            return *it;
        }
        file.close();
    }
    throw ServerException(NotFound);
}

std::vector<std::string> getFilesInDirectory(std::string const & directoryPath) {
    std::vector<std::string> files;

    DIR* dir = opendir(directoryPath.c_str());
    if (dir == NULL) {
        Console::error("Error opening directory: " + directoryPath);
        throw ServerException(ServerError);
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        // Ignore "." and ".." entries
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            if (entry->d_type == DT_DIR)
                files.push_back(entry->d_name + std::string("/"));
            else
                files.push_back(entry->d_name);
        }
    }

    closedir(dir);

    return files;
}

std::string generateHtmlListing(const std::vector<std::string>& files) {
    std::string htmlFilePath = "/tmp/listing.html";

    std::ofstream htmlFile(htmlFilePath);

    if (!htmlFile.is_open()) {
        Console::error("Error opening HTML file for writing listings");
        return NULL;
    }
    
    htmlFile << "<!DOCTYPE html>\n<html>\n<head>\n<title>File Listing</title>\n</head>\n<body>\n";
    htmlFile << "<ul>\n";
    for (size_t i = 0; i < files.size(); ++i) {
        htmlFile << "  <li><a href='" << files[i] << "'>" << files[i] << "</a></li>\n";
    }
    htmlFile << "</ul>\n";
    htmlFile << "</body>\n</html>\n";
    htmlFile.close();

    return htmlFilePath;
}

std::string Response::getFilePath(Server const & server, Route const & route, Request const & req) {
    if (route.getRouteType() == Route::FILE) {
        return route.getRoot() + "/" + route.getPath();
    }
    
    std::string filePath;
    std::string root = route.getRoot().empty() ? server.getRoot() : route.getRoot();
    try {
        std::string index = tryFiles(server, route, req, root);
        
        filePath = root + "/" + index;
        return filePath;
    } catch (ServerException & e) {
        if (e.what() == toString(NotFound) \
            && (route.getAllowListing() || (route.getRouteType() == Route::DIRECTORY && server.getAllowListing()))) {
            std::vector<std::string> files = getFilesInDirectory(root);
            return generateHtmlListing(files);
        }
        throw ServerException(Forbidden);
    }
}

void Response::handleGet(Server const & server, Route const & route, Request const & req) {
    std::string filePath = getFilePath(server, route, req);
    removeConsecutiveChars(filePath, '/');
    Console::info("Serving file: " + filePath);
    serveStaticFile(filePath);
}

void Response::handleResponse(Request const & req) {
    try {
        Server server = getServer(req);
        Route route = getRoute(server, req);

        if (req.getMethod() == "GET") {
            handleGet(server, route, req);
        }
        else if (req.getMethod() == "POST") {
            std::cout << "POST" << std::endl;
        }
        else if (req.getMethod() == "DELETE") {
            std::cout << "DELETE" << std::endl;
        }
        else {
            throw ServerException(NotImplemented);
        }
    } catch (ServerException & e) {
        code = static_cast<HttpStatusCode>(std::atoi(e.what()));
        body = getStatusMessage(code);
        Console::warning(getStatusMessage(code));
    } catch (std::exception & e) {
        code = ServerError;
        body = getStatusMessage(code);
        Console::error(getStatusMessage(code));
    }
}