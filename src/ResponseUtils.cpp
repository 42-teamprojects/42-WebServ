/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseUtils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusufisawi <yusufisawi@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 12:22:45 by yelaissa          #+#    #+#             */
/*   Updated: 2023/12/06 18:47:49 by yusufisawi       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::string Response::getStatusMessage(HttpStatusCode code) {
    switch (code) {
        case OK: return "OK";
        case BadRequest: return "Bad Request";
        case Unauthorized: return "Unauthorized";
        case Forbidden: return "Forbidden";
        case NotFound: return "Not Found";
        case Conflict: return "Conflict";
        case MethodNotAllowed: return "Method Not Allowed";
        case RequestURITooLong: return "Request-URI Too Long";
        case RequestEntityTooLarge: return "Request Entity Too Large";
        case NotImplemented: return "Not Implemented";
        case NoContent: return "No Content";
        case ServerError: return "Internal Server Error";
        case BadGateway: return "Bad Gateway";
        default: return "Unknown";
    }
}

std::string Response::getResponse() {
    std::stringstream ss;
    ss << "HTTP/1.1 " << toString(code) << " " << getStatusMessage(code) << "\r\n";
    ss << "Connection: keep-alive\r\n";
    ss << "Server: webserv\r\n";
    ss << "Date: " << getDateGMT() << "\r\n";
    for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++) {
        ss << it->first << ": " << it->second << "\r\n";
    }
    ss << "\r\n";
    ss << body;
    return ss.str();
}

#include <fstream>

void Response::readContent(std::string const &filePath, HttpStatusCode code) {
    std::ifstream file(filePath);
    if (file.is_open())
    {
        std::stringstream buffer;
        std::string line;
        while (std::getline(file, line))
            buffer << line << std::endl;
        body = buffer.str();
        file.close();
    }
    else
    {
        code = code == OK ? NotFound : code;
        body = "<html><h1 align='center'>" + toString(code) + " " + getStatusMessage(code) + "</h1></html>";
    }
    headers["Content-Length"] = toString(body.size());
}

Server Response::getServer() {
    std::vector<Server>::iterator it = Config::find(request->getHost(), request->getPort());
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

Route Response::findBestMatch(Route & route, std::string const & resource) {
    if (isDirectory(route.getRoot() + resource)) { // Check if resource is a directory
        if (resource.back() != '/') {
            if (request->getMethod() == "DELETE") {
                throw ServerException(Conflict);
            }
            else {
                headers["Location"] = route.getPath() + resource + "/";
                throw ServerException(MovedPermanently);
            }
        }
        Route newRoute(route.getRoot() + resource, route.getPath() + resource, Route::DIRECTORY);
        newRoute.setAllowListing(route.getAllowListing());
        return newRoute;
    }
    else if (isFile(route.getRoot() + resource)) { // Check if resource is a file
        Route newRoute(route);
        newRoute.setPath(resource);
        newRoute.setRouteType(Route::FILE);
        return newRoute;
    }
    throw ServerException(NotFound);
}

Route Response::findBestMatch(Server & server, std::string const & resource) {
    if (isDirectory(server.getRoot() + resource)) { // Check if resource is a directory
        if (resource.back() != '/') { // Check if resource ends with a slash (if not, redirect to resource/
            if (request->getMethod() == "DELETE") {
                throw ServerException(Conflict);
            }
            else {
                headers["Location"] = resource + "/";
                throw ServerException(MovedPermanently);
            }
        }
        Route newRoute(server.getRoot() + resource, resource, Route::DIRECTORY);
        newRoute.setAllowListing(server.getAllowListing());
        return newRoute;
    }
    else if (isFile(server.getRoot() + resource)) { // Check if resource is a file
        return Route(server.getRoot(), resource, Route::FILE);
    }
    throw ServerException(NotFound);
}

void Response::checkRedirection(Route const & route) {
    if (!route.getRedirect().empty()) {
        headers["Location"] = "/" + route.getRedirect();
        throw ServerException(MovedPermanently);
    }
}

void Response::checkMethods(Route const & route) {
    std::vector<std::string> methods = route.getMethods();
    if (!methods.empty() && std::find(methods.begin(), methods.end(), request->getMethod()) == methods.end()) {
        throw ServerException(MethodNotAllowed);
    }
}

std::string Response::tryFiles(Server const & server, Route const & route, std::string & root) {
    if (route.getRouteType() == Route::DIRECTORY)
        root = route.getRoot();

    std::vector<std::string> indexes;
    if (route.getRouteType() == Route::FILE)
        indexes.push_back(route.getPath());
    else
        indexes = route.getIndex().empty() \
            ? (server.getIndex().empty() ? std::vector<std::string>() : server.getIndex()) \
            : route.getIndex();
    
    indexes.push_back("index.html");
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
    if (route.getRouteType() != Route::FILE)
        throw ServerException(Forbidden);
    throw ServerException(NotFound);
}
