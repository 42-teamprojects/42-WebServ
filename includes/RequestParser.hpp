/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 19:59:15 by msodor            #+#    #+#             */
/*   Updated: 2023/11/15 15:21:35 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
  Request();
  ~Request();
  
  void parse(std::string request);
  void parseMethod(std::string line);
  void checkIfChunked();

  std::string getMethod();
  std::string geturi();
  std::string getVersion();
  std::map<std::string, std::string> getHeaders();
  std::string getBody();
};
