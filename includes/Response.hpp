/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:46:37 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/18 14:44:57 by yelaissa         ###   ########.fr       */
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

    const char *getStatusMessage(HttpStatusCode code);

    std::string getResponse();

    void serveStaticFile(std::string const &filePath);

    Server getServer(Request const &req);

    Route getRoute(Server &server, Request const &req);

    void handleResponse(Request const &request);
};
