/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 10:56:24 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/19 20:33:02 by yelaissa         ###   ########.fr       */
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

Route Response::getRoute(Server & server, Request const & req) {
    // Check if request is for default "/"
    if (req.getUri().back() == '/') {
        return Route();
    }
    // Check if request is for a resource file
    std::string resource = getRequestedResource(req.getUri());
    if (resource.find(".") != std::string::npos)
        return Route("/" + resource);

    std::vector<Route>::iterator it = server.find(req.getUri());
    if (it == server.end()) { // Get matched route for request
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
    if (!route.getPath().empty() && route.getPath().find('.') != std::string::npos) {
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

std::string Response::getFilePath(Server const & server, Route const & route, Request const & req) {
    std::string filePath;
    std::string root = route.getRoot().empty() ? server.getRoot() : route.getRoot();
    std::string index = tryFiles(server, route, req, root);

    filePath = root + "/" + index;
    removeConsecutiveChars(filePath, '/');
    Logger::info("Serving file: " + filePath);
    return filePath;
}

void Response::handleGet(Server const & server, Route const & route, Request const & req) {
    std::string filePath = getFilePath(server, route, req);
    serveStaticFile(filePath);
}

void Response::handleResponse(Request const & req) {
    req.print();
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
        Logger::warning(getStatusMessage(code));
    } catch (std::exception & e) {
        code = ServerError;
        body = getStatusMessage(code);
        Logger::error(getStatusMessage(code));
    }
}