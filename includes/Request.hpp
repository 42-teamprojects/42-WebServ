/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 19:59:15 by msodor            #+#    #+#             */
/*   Updated: 2023/11/16 20:07:33 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

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

  bool isChunked;

public:
  Request(std::string request);
  ~Request();

  enum HttpStatusCode  parse(std::string request);
  int   parseStatusLine(std::string& line);
  int   uriCharCheck(std::string& uri);
  int   uriLenCheck(std::string& uri);
  void  parseHeaders(std::string& line);
  void  parseHost();
  void  checkIfChunked();
  void  print();

  std::string getMethod() const;
  std::string getUri() const;
  std::string getVersion() const;
  std::map<std::string, std::string> getHeaders() const;
  std::string getBody() const;
};
