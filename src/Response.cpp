/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 10:56:24 by yelaissa          #+#    #+#             */
/*   Updated: 2023/12/06 18:11:12 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "utils.hpp"

Response::Response(std::string const & buffer) {
    mimes = Mimes();
    isListing = false;
    request = new Request(buffer);
    code = OK;
    handleResponse();
}

Response::~Response() {
    delete request;
}

Route Response::deepSearch(Server & server, std::string const & resource) {
    std::vector<Route> routes = server.getRoutes();
    for (std::vector<Route>::iterator it = routes.begin(); it != routes.end(); it++) {
        std::pair<std::string, bool>   matchedPath = getMatchedPath(it->getPath(), resource);
        if (matchedPath.second) {
            std::string newResource = "/" + matchedPath.first;
            removeConsecutiveChars(newResource, '/');
            Route route = findBestMatch(*it, newResource);
            checkRedirection(*it);
            checkMethods(*it);
            return route;
        }
    }
    return findBestMatch(server, resource); 
}

Route Response::getRoute(Server & server) {
    std::string resource = getRequestedResource(request->getUri());
    
    // Get matched route for request
    std::vector<Route>::iterator it = server.find(resource);
    if (it == server.end()) {
        return deepSearch(server, resource);
    }
    if (resource.back() != '/') {
        headers["Location"] = request->getUri() + "/";
        throw ServerException(MovedPermanently);
    }
    checkRedirection(*it);
    checkMethods(*it);
    if (server.getClientMaxBodySize() != 0 && request->getMethod() != "GET" && \
        (size_t)request->getContentLength() > server.getClientMaxBodySize())
        throw ServerException(RequestEntityTooLarge);
    return *it;
}

std::string Response::getFilePath(Server const & server, Route const & route) {
    if (route.getRouteType() == Route::FILE)
        return route.getRoot() + "/" + route.getPath();
    
    try {
        std::string root, index;
        root = route.getRoot().empty() ? server.getRoot() : route.getRoot();
        index = tryFiles(server, route, root);
        return root + "/" + index;
    } catch (ServerException & e) {
        // check if listing is allowed
        if (e.getCode() == Forbidden && (route.getAllowListing() || server.getAllowListing()) && request->getMethod() == "GET") {
            std::vector<std::string> files = getFilesInDirectory(route.getRoot(), route.getPath());
            return (isListing = true, body = generateHtmlListing(files), "");
        }
        throw ServerException(e.getCode());
    }
}

void Response::handleGet(Server const & server, Route const & route) {
    std::string filePath = getFilePath(server, route);
    if (isListing)
        return;
    removeConsecutiveChars(filePath, '/');
    headers["Content-Type"] = mimes[getFileExt(filePath)];
    if (!route.getCgi().empty()) {
        Console::info("Serving CGI file: " + filePath);
        Cgi cgi(route, filePath, *request);
        body = cgi.getResponseBody();
        return; 
    }
    Console::info("Serving file: " + filePath);
    readFile(filePath, OK);
}

void Response::handleDelete(Server const & server, Route const & route) {
    std::string filePath = getFilePath(server, route);
    removeConsecutiveChars(filePath, '/');
    if (!route.getCgi().empty()) {
        Console::info("Running CGI: " + filePath);
        Cgi cgi(route, filePath, *request);
        body = cgi.getResponseBody();
        return; 
    }
}

/* 
TODO:
    - body max size
 */
void Response::handleResponse() {
    Server server = getServer();
    try {
        Route route = getRoute(server);
        if (request->getMethod() == "GET") {
            handleGet(server, route);
        }
        else if (request->getMethod() == "POST") {
            readBody();
        }
        else if (request->getMethod() == "DELETE") {
            handleDelete(server, route);
        }
        else {
            throw ServerException(NotImplemented);
        }
    } catch (ServerException & e) {
        code = e.getCode();
        readFile(server.getErrorPages()[code], code);
        Console::warning(request->getUri() + " : " + getStatusMessage(code));
    } catch (std::exception & e) {
        code = ServerError;
        readFile(server.getErrorPages()[code], code);
        Console::error(e.what());
    }
}

void Response::readBody() {
    const std::string& contentType = request->getContentType();
    const std::string& body = request->getBody();
    
    if (contentType == "application/x-www-form-urlencoded") {
        processUrlEncodedBody(body);
    } else if (contentType == "multipart/form-data") {
        processMultipartFormDataBody(body);
    }
}

void Response::processUrlEncodedBody(const std::string& body) {
    std::map<std::string, std::string> queryStrings;
    std::vector<std::string> params = ft_split(body, "&");

    for (std::vector<std::string>::const_iterator it = params.begin(); it != params.end(); ++it) {
        std::vector<std::string> param = ft_split(*it, "=");
        if (param.size() == 2) {
            queryStrings[param[0]] = param[1];
        }
    }
}

void Response::processMultipartFormDataBody(const std::string& body) {
    std::map<std::string, std::string> queryStrings;
    std::string boundary = request->getBoundary();
    std::vector<std::string> params = split(body, "--" + boundary);

    for (std::vector<std::string>::const_iterator it = params.begin(); it != params.end(); ++it) {
        std::istringstream ss(*it);
        std::string line;

        std::getline(ss, line);

        if (line.find("filename") != std::string::npos) {
            std::cout << "file found" << std::endl;
            processFileUpload(ss, line);
        } else if (line.find("name") != std::string::npos){
            processFormField(ss, line, queryStrings);
        }
    }
}

void Response::processFileUpload(std::istringstream& ss, const std::string& line) {
    int len = line.find("\"", line.find("filename") + 10) - line.find("filename") - 10;
    std::string filename = "www/uploads/uploads/" + line.substr(line.find("filename") + 10, len);
    std::cout << "filename: " << filename << std::endl;
    std::ofstream file(filename.c_str());

    std::string content;
    std::getline(ss, content);
    std::getline(ss, content);
    while (std::getline(ss, content)){
        file << content << "\n";
    }
}

void Response::processFormField(std::istringstream& ss, const std::string& line, std::map<std::string, std::string>& queryStrings) {
    int len = line.find("\"", line.find("name") + 6) - line.find("name") - 6;
    std::string name = line.substr(line.find("name") + 6, len);

    std::string content;
    std::getline(ss, content);
    std::getline(ss, content);
    std::getline(ss, content, '\0');
    queryStrings[name] = content;
    std::cout << "name: " << name << std::endl;
    std::cout << "content: " << content << std::endl;
}
