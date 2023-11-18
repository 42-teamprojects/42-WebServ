/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 10:56:24 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/18 12:34:06 by yelaissa         ###   ########.fr       */
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
        body = "404 Not Found";
    }
}

Server Response::getServer(Request const & req) {
    std::vector<Server>::iterator it = Config::find(req.getHost(), req.getPort());
    if (it != Config::end()) {
        return *it;
    }
    return *(Config::begin());
}

Route Response::getRoute(Server & server, std::string const & path) {
    if (path.back() == '/' && !(server.getRoot().empty() && server.getIndex().empty())) {
        return Route();
    }
    std::vector<Route>::iterator it = server.find(path);
    if (it == server.end()) {
        throw ServerException(NotFound);
    }
    return *it;
}

void Response::handleResponse(Request const & request) {
    Server server = getServer(request);
    try {
        Route route = getRoute(server, request.getUri());
        std::string filePath = server.getRoot() + route.getPath();
        if (request.getUri().back() == '/') {
            filePath += "/" + server.getIndex()[0];
        }
        std::cout << filePath << std::endl;
        serveStaticFile(filePath);
    } catch (ServerException & e) {
        if (e.what() == toString(NotFound)) {
            code = NotFound;
            body = "404 Not Found";
        } else {
            code = NotImplemented;
            body = "501 Not Implemented";
        }
    }
}