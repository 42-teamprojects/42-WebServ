/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msodor <msodor@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 22:07:44 by msodor            #+#    #+#             */
/*   Updated: 2023/12/01 21:15:12 by msodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(std::string request) : isChunked(false), contentLength(-1), statusCode(OK)
{
    parse(request);
    parseContentType();
    parseBoundary();
    print();
}

Request::~Request()
{
}

std::string methods[] = {
    "GET",
    "POST",
    "PUT",
    "DELETE",
    "HEAD",
    "CONNECT",
    "OPTIONS",
    "TRACE",
    "PATCH",
};

int Request::uriCharCheck(std::string& uri)
{
    if (uri.empty())
        return 1;
    std::string allowed = "-._~:/?#[]@!$&'()*+,;=%";
    std::string::iterator it = uri.begin();
    for (; it != uri.end(); ++it)
    {
        if (!isalnum(*it) && allowed.find(*it) == std::string::npos)
            return 1;
    }
    return 0;
}

int Request::uriLenCheck(std::string& uri)
{
    if (uri.length() > 2048)
    {
        this->statusCode = RequestURITooLong;
        return 1;
    }
    return 0;
}

int Request::versionCheck(std::string& version)
{
    if (version != "HTTP/1.1")
        return 1;
    return 0;
}

int Request::methodCheck(std::string& method)
{
    if (std::find(std::begin(methods), std::end(methods), method) == std::end(methods))
        return 1;
    return 0;
}

void Request::statusLineCheck()
{
    if (uriLenCheck(this->uri))
    {
        return ;
    }
    if (uriCharCheck(this->uri) ||
        versionCheck(this->version) ||
          methodCheck(this->method))
    {
        this->statusCode = BadRequest;
        return ;
    }
}

void Request::saveEncoding()
{
    std::map<std::string, std::string>::iterator it = headers.find("Transfer-Encoding");
    if (it != headers.end())
    {
        if (it->second == "chunked")
            this->isChunked = true;
        else
        {
            this->statusCode = NotImplemented;
            return ;
        }
    }
    std::map<std::string, std::string>::iterator it2 = headers.find("Content-Length");
    if (it2 != headers.end())
        this->contentLength = std::atoi(it2->second.c_str());
}

int Request::encodingCheck()
{
    saveEncoding();
    if (this->isChunked == 0 && this->contentLength == -1 && this->method == "POST")
    {
        this->statusCode = BadRequest;
        return 1;
    }
    return 0;
}

void Request::unchunkBody(std::string& chunkedBody)
{
    std::string body;
    std::istringstream input(chunkedBody);

    char c;
    while (input.get(c)) {
        // Read the chunk size
        std::string chunk_size_hex;
        while (c != '\r') {
            chunk_size_hex += c;
            input.get(c);
        }
        input.get(c); // '\n'
        int chunk_size = std::stoi(chunk_size_hex, 0, 16);
        if (chunk_size == 0) {
            break;
        }
        // Read the chunk
        std::string chunk(chunk_size, '\0');
        input.read(&chunk[0], chunk_size);
        input.get(c); // '\r'
        input.get(c); // '\n'
        body += chunk;
    }
    this->body = body;
}

void Request::parseContentType()
{
    std::map<std::string, std::string>::iterator it = headers.find("Content-Type");
    if (it != headers.end())
    {
        std::string value = it->second;
        std::stringstream ss(value);
        std::getline(ss, this->contentType, ';');
    }
}

void Request::parseBoundary()
{
    std::map<std::string, std::string>::iterator it = headers.find("Content-Type");
    if (it != headers.end() && this->contentType == "multipart/form-data")
    {
        std::string value = it->second;
        std::stringstream ss(value);
        std::getline(ss, this->boundary, '=');
        std::getline(ss, this->boundary);
    }
}

void Request::parseStatusLine(std::string& line)
{
    std::string method;
    std::string uri;
    std::string version;
    std::stringstream ss(line);
    ss >> method;
    ss >> uri;
    ss >> version;
    
    if (methodCheck(method) || uriCharCheck(uri) || versionCheck(version))
    {
        this->statusCode = BadRequest;
        return ;
    }
    this->method = method;
    this->uri = uri;
    this->version = version;
}

void Request::parseHeaders(std::string& line)
{
    std::string key;
    std::string value;
    std::stringstream ss(line);

    std::getline(ss, key, ':');
    std::getline(ss, value);
    trim(value);
    headers[key] = value;
}

void Request::parse(std::string request)
{
    std::string line;
    std::stringstream req(request);
    std::getline(req, line);
    //pars status line
    parseStatusLine(line);
    statusLineCheck();
    //pars headers
    while (std::getline(req, line) && line != "\r" && statusCode == OK)
        parseHeaders(line);
    if (statusCode != OK)
        return ;
    encodingCheck();
    //pars host
    parseHost();
    //pars body
    std::string body;
    std::getline(req, body, '\0');
    if (this->isChunked)
        unchunkBody(body);
    else if (this->contentLength > 0)
        this->body = body;
}

void    Request::parseHost()
{
    std::map<std::string, std::string>::iterator it = headers.find("Host");
    if (it != headers.end())
    {
        std::string port;
        std::string value = it->second;
        std::stringstream ss(value);
        std::getline(ss, this->host, ':');
        std::getline(ss, port);
        if (port.empty())
            this->port = 80;
        else
            this->port = std::atoi(port.c_str());
    }   
}

std::string Request::getMethod() const {
    return method;
}

std::string Request::getUri() const {
    return uri;
}

std::string Request::getVersion() const {
    return version;
}

std::map<std::string, std::string> Request::getHeaders() const {
    return headers;
}

std::string Request::getHost() const {
    return host;
}

int Request::getPort() const {
    return port;
}
std::string Request::getBody() const {
    return body;
}

std::string Request::getContentType() const {
    return contentType;
}

std::string Request::getBoundary() const {
    return boundary;
}

int Request::getContentLength() const {
    return contentLength;
}

void Request::print() const
{
    // std::cout << "---------------------" << std::endl;
    // std::cout << "Method : " << getMethod() << std::endl;
    // std::cout << "Uri : " << getUri() << std::endl;
    // std::cout << "Version : " << getVersion() << std::endl;
    // std::cout << "Host : " << host << std::endl;
    // std::cout << "Port : " << port << std::endl;
    // std::cout << "Encoding : " << contentType << std::endl;
    // std::cout << "Boundary : " << boundary << std::endl;
    // std::map<std::string, std::string> headers = getHeaders();
    // std::cout << "Headers : " << std::endl;
    // std::map<std::string, std::string>::iterator it = headers.begin();
    // for (; it != headers.end(); ++it)
    //     std::cout << "  " << it->first << " : " << it->second << '\n';
    std::cout << "Body : " << getBody() << std::endl;
    // std::cout << "Status : " << statusCode << std::endl;
}
