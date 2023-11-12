/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msodor <msodor@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 19:59:15 by msodor            #+#    #+#             */
/*   Updated: 2023/11/12 21:15:22 by msodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>

class Request
{
private:
  std::string method;
  std::string path;
  std::string version;
  std::map<std::string, std::string> headers;
  std::string body;
public:
  Request(/* args */);
  ~Request();
  
  void parse(std::string request);
  void parseMethod(std::string line);

  std::string getMethod();
  std::string getPath();
  std::string getVersion();
  std::map<std::string, std::string> getHeaders();
  std::string getBody();
};

Request::Request(/* args */)
{
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
  std::string path;
  std::string version;
  std::stringstream ss(line);
  ss >> method;
  ss >> path;
  ss >> version;
  if (std::find(std::begin(methods), std::end(methods), method) != std::end(methods))
  {
    this->method = method;
    this->path = path;
    this->version = version;
  }
  else
  {
    throw std::invalid_argument("Invalid method");
  }
}


void Request::parse(std::string request)
{
  std::string line;
  std::stringstream req(request);
  std::getline(req, line);
  try {
    parseMethod(line);
  }
  catch(const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

std::string Request::getMethod(){
  return method;
}

std::string Request::getPath(){
  return path;
}

std::string Request::getVersion(){
  return version;
}

std::map<std::string, std::string> Request::getHeaders(){
  return headers;
}

std::string Request::getBody(){
  return body;
}

