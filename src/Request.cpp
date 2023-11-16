/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 22:07:44 by msodor            #+#    #+#             */
/*   Updated: 2023/11/16 12:09:25 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(std::string request) : isCunked(false)
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

void Request::parseMethod(std::string line)
{
  std::string method;
  std::string uri;
  std::string version;
  std::stringstream ss(line);
  ss >> method;
  ss >> uri;
  ss >> version;
  if (std::find(std::begin(methods), std::end(methods), method) != std::end(methods))
  {
    this->method = method;
    this->uri = uri;
    this->version = version;
  }
  else
  {
    throw std::invalid_argument("Invalid method");
  }
}

void Request::checkIfChunked()
{
  std::map<std::string, std::string>::iterator it = headers.find("Transfer-Encoding");
  if (it != headers.end())
  {
    if (it->second == "chunked")
      this->isCunked = true;
  }
}

void Request::parse(std::string request)
{
  std::string line;
  std::stringstream req(request);
  std::getline(req, line);
  try
  {
    parseMethod(line);
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << std::endl;
    return;
  }
  while (std::getline(req, line))
  {
    std::string key;
    std::string value;
    std::stringstream ss(line);
    std::getline(ss, key, ':');
    if (key[0] == ' ')
      key.erase(0, 1);
    std::getline(ss, value);
    if (value.empty())
      break;
    trim(value);
    headers[key] = value;
  }
  std::getline(req, body, '\0');
}

std::string Request::getMethod()
{
  return method;
}

std::string Request::getUri()
{
  return uri;
}

std::string Request::getVersion()
{
  return version;
}

std::map<std::string, std::string> Request::getHeaders()
{
  return headers;
}

std::string Request::getBody()
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
    for (; it != headers.end(); ++it){
      std::cout << "      " << it->first << " : " << it->second << '\n';
    }
    std::cout << "Body : " << getBody() << std::endl;
}
