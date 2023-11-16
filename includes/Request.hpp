/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 19:59:15 by msodor            #+#    #+#             */
/*   Updated: 2023/11/16 18:57:01 by yelaissa         ###   ########.fr       */
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
  std::map<std::string, std::string> headers;
  std::string body;

  bool isCunked;

public:
  Request(std::string request);
  ~Request();

  void parse(std::string request);
  void parseMethod(std::string line);
  void checkIfChunked();
  void print();

  std::string getMethod();
  std::string getUri();
  std::string getVersion();
  std::map<std::string, std::string> getHeaders();
  std::string getBody();
};
