/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msodor <msodor@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:46:37 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/17 00:21:36 by msodor           ###   ########.fr       */
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
    NotImplemented = 501
}  ;

class Response
{
private:
  std::string status;
  std::map<std::string, std::string> headers;
  std::string body;

public:
    Response(std::string const &);
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

    
};
