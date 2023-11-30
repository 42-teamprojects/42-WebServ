/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msodor <msodor@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 19:59:15 by msodor            #+#    #+#             */
/*   Updated: 2023/11/30 13:45:35 by msodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include "utils.hpp"

class Request
{
private:
    std::string method;
    std::string uri;
    std::string version;
    std::string host;
    int port;
    std::map<std::string, std::string> headers;
    std::string body;
    
    bool    isChunked;
    int     contentLength;

    std::string encoding;
    std::string boundary;

    HttpStatusCode  statusCode;

public:
    Request(std::string request);
    ~Request();
    
    void    parse(std::string request);
    void    parseStatusLine(std::string& line);
    void    parseHeaders(std::string& line);
    void    parseHost();
    void    saveEncoding();
    int     uriCharCheck(std::string& uri);
    int     uriLenCheck(std::string& uri);
    int     versionCheck(std::string& version);
    int     methodCheck(std::string& method);
    void    statusLineCheck();
    int     encodingCheck();

    void    unchunkBody(std::string& body);
    void    parseEncoding();
    void    parseBoundary();
    

    std::string getHost() const;
    int getPort() const;
    void print() const;
       
    std::string getMethod() const;
    std::string getUri() const;
    std::string getVersion() const;
    std::map<std::string, std::string> getHeaders() const;
    std::string getBody() const;
};
