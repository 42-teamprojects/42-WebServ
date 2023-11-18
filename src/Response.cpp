/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 10:56:24 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/18 15:50:30 by yelaissa         ###   ########.fr       */
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

Route Response::getRoute(Server & server, Request const & req) {
    if (req.getUri().back() == '/' && !(server.getRoot().empty() && server.getIndex().empty())) {
        return Route();
    }
    std::vector<Route>::iterator it = server.find(req.getUri());
    if (it == server.end()) { // Get matched route for request
        throw ServerException(NotFound);
    }
    if (!it->getRedirect().empty()) { // Check if route have redirection
        headers["Location"] = it->getRedirect();
        throw ServerException(MovedPermanently);
    }
    std::vector<std::string> methods = it->getMethods(); // Check if route have allowed methods
    if (std::find(methods.begin(), methods.end(), req.getMethod()) == methods.end()) {
        throw ServerException(MethodNotAllowed);
    }
    return *it;
}

void Response::handleResponse(Request const & request) {
    try {
        Server server = getServer(request);
        Route route = getRoute(server, request);
        std::string filePath = server.getRoot() + route.getPath();
        if (request.getUri().back() == '/') {
            filePath += "/" + server.getIndex()[0];
        }
        std::cout << filePath << std::endl;
        serveStaticFile(filePath);
    } catch (ServerException & e) {
        code = static_cast<HttpStatusCode>(std::atoi(e.what()));
        body = getStatusMessage(code);
    } catch (std::exception & e) {
        code = ServerError;
        body = getStatusMessage(code);
        std::cout << e.what() << std::endl;
    }
}