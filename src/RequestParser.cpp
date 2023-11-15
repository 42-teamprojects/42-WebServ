/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msodor <msodor@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 22:07:44 by msodor            #+#    #+#             */
/*   Updated: 2023/11/15 14:30:28 by msodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"

Request::Request()
{
  this->isCunked = false;
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

void  Request::checkIfChunked()
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
  try {
    parseMethod(line);
  }
  catch(const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return;
  }
  int i = 0;
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
    headers[key] = value;
  }
  std::getline(req, body, '\0');
}

std::string Request::getMethod(){
  return method;
}

std::string Request::geturi(){
  return uri;
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

int main(void) {
  std::string request = "GET / HTTP/1.1\r\n\
  Host: localhost:8080\r\n\
  Connection: keep-alive\r\n\
  Upgrade-Insecure-Requests: 1\r\n\
  Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n\
  User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_3) AppleWebKit/604.5.6 (KHTML, like Gecko) Version/11.0.3 Safari/604.5.6\r\n\
  Accept-Language: en-us\r\n\
  DNT: 1\r\n\
  Accept-Encoding: gzip, deflate\r\n\
  \r\n\
  hello this is a body";
  Request req;
  req.parse(request);
  std::cout << "Method ==>" << req.getMethod() << std::endl;
  std::cout << "uri ==>" << req.geturi() << std::endl;
  std::cout << "Version ==>" << req.getVersion() << std::endl;
  std::map<std::string, std::string> headers = req.getHeaders();
  std::cout << "Headers ==>" << std::endl;
  std::map<std::string, std::string>::iterator it = headers.begin();
  int i = 0;
  for (; it != headers.end(); ++it){
    std::cout << it->first << " => " << it->second << '\n';
  }
  std::cout << "Body ==>" << req.getBody() << std::endl;
  return 0;
}