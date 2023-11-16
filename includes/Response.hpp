/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:46:37 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/16 20:05:08 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

enum HttpStatusCode {
    OK = 200,
    MovedPermanently = 301,
    BadRequest = 400,
    Unauthorized = 401,
    Forbidden = 403,
    NotFound = 404,
    MethodNotAllowed = 405,
    RequestURITooLong = 414,
    RequestEntityTooLarge = 413,
    NotImplemented = 501,
    ServerError = 500
};

class Response
{
private:
  HttpStatusCode code;
  std::map<std::string, std::string> headers;
  std::string body;

  std::string response;

public:
    Response(std::string const & buffer) {
        Request request(buffer);
        response = handleResponse(request);
    }
    ~Response();

    const char* getStatusMessage(HttpStatusCode code) {
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

    std::string getResponse() {
        std::stringstream ss;
        ss << " HTTP/1.1" << toString(code) << " " << getStatusMessage(code) << "\r\n";
        for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++) {
            ss << it->first << ": " << it->second << "\r\n";
        }
        ss << "\r\n";
        ss << body;
        return ss.str();
    }

    // serve a static website from a file and return the response
    void serveStaticFile(std::string const &filePath) {
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

    Server getServer(Request const & request) {
        (void) request;
        std::vector<Server>::iterator it = Config::find("127.0.0.1", 8080);
        if (it != Config::getServers().end()) {
            return *it;
        }
        return *(Config::begin());
    }
   
    Route getRoute(Server & server, std::string const & path) {
        std::vector<Route>::iterator it = server.find(path);
        if (it == server.end() && (server.getRoot().empty() || server.getIndex().empty())) {
            throw ServerException(NotFound);
        }
        return *it;
    }

    std::string handleResponse(Request & request) {
        Server server = getServer(request);
        try {
            Route route = getRoute(server, request.getUri());

        } catch (ServerException & e) {
            std::cerr << e.what() << std::endl;
            // std::string filePath = server.getRoot() + request.getUri();
            // if (request.getUri().back() == '/') {
            //     filePath += server.getIndex()[0];
            // }
            // serveStaticFile(filePath);
        }
    }
};
