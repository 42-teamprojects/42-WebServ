/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:46:37 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/27 12:18:57 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

class Request;

class Response
{
private:
    Request *request;
    HttpStatusCode code;
    std::map<std::string, std::string> headers;
    std::string body;

    bool isListing;
public:
    Response(std::string const &buffer);
    ~Response();

    std::string getStatusMessage(HttpStatusCode);

    std::string getResponse();

    void readFile(std::string const &, HttpStatusCode);

    Server getServer();

    Route getRoute(Server &);

    void handleResponse();

    void handleGet(Server const &, Route const &);

    std::string getRequestedResource(std::string const &);

    std::string getFilePath(Server const &, Route const &);

    std::string tryFiles(Server const &, Route const &, std::string &);
    Route findBestMatch(Server & server, std::string const & resource);
    Route findBestMatch(Route & route, std::string const & resource);

    void checkRedirection(Route const & route);
    void checkMethods(Route const & route);
    Route deepSearch(Server & server, std::string const & resource);
    
};
