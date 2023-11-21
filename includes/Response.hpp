/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:46:37 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/21 22:03:04 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

class Request;

class Response
{
private:
    HttpStatusCode code;
    std::map<std::string, std::string> headers;
    std::string body;

public:
    Response(std::string const &buffer);
    ~Response();

    std::string getStatusMessage(HttpStatusCode);

    std::string getResponse();

    void serveStaticFile(std::string const &, HttpStatusCode);

    Server getServer(Request const &);

    Route getRoute(Server &, Request const &);

    void handleResponse(Request const &);

    void handleGet(Server const &, Route const &, Request const &);

    std::string getRequestedResource(std::string const &);

    std::string getFilePath(Server const &, Route const &, Request const &);

    std::string tryFiles(Server const &, Route const &, Request const &, std::string const &);
};
