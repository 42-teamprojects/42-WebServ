/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msodor <msodor@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 22:07:44 by msodor            #+#    #+#             */
/*   Updated: 2023/11/16 18:56:48 by msodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(std::string request) : isChunked(false)
{
    parse(request);
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
        if (!isalnum(*it) || allowed.find(*it) == std::string::npos)
            return 1;
    }
    return 0;
}

int Request::uriLenCheck(std::string& uri)
{
    if (uri.length() > 2048)
        return 1;
    return 0;
}

int Request::parseStatusLine(std::string& line)
{
    std::string method;
    std::string uri;
    std::string version;
    std::stringstream ss(line);
    ss >> method;
    ss >> uri;
    ss >> version;
    if (std::find(std::begin(methods), std::end(methods), method) == std::end(methods)\
    || version != "HTTP/1.1" || uriCharCheck(uri))
        return 1;
    else {
        this->method = method;
        this->uri = uri;
        this->version = version;
    }
    return 0;
}

// void Request::checkIfChunked()
// {
//     std::map<std::string, std::string>::iterator it = headers.find("Transfer-Encoding");
//     if (it != headers.end())
//     {
//         if (it->second == "chunked")
//             this->isCunked = true;
//     }
// }

void Request::parseHeaders(std::string& line)
{
    
}

void Request::parse(std::string request)
{
    std::string line;
    std::stringstream req(request);
    std::getline(req, line);
    parseStatusLine(line);
    while (std::getline(req, line))
    {
        std::string key;
        std::string value;
        std::stringstream ss(line);
        if (line == "\r")
            break;
        std::getline(ss, key, ':');
        std::getline(ss, value);
        trim(value);
        headers[key] = value;
    }
    std::getline(req, body, '\0');
}

void    Request::hostPort()
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

std::string Request::getMethod() const
{
    return method;
}

std::string Request::getUri() const
{
    return uri;
}

std::string Request::getVersion() const
{
    return version;
}

std::map<std::string, std::string> Request::getHeaders() const
{
    return headers;
}

std::string Request::getBody()  const
{
    return body;
}

void Request::print()
{
    std::cout << "---------------------" << std::endl;
    std::cout << "Method : " << getMethod() << std::endl;
    std::cout << "Uri : " << getUri() << std::endl;
    std::cout << "Version : " << getVersion() << std::endl;
    std::map<std::string, std::string> headers = getHeaders();
    std::cout << "Headers : " << std::endl;
    std::map<std::string, std::string>::iterator it = headers.begin();
    for (; it != headers.end(); ++it)
    {
        std::cout << "  " << it->first << " : " << it->second << '\n';
    }
    std::cout << "Body : " << getBody() << std::endl;
}
