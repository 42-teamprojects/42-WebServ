/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 11:03:20 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/08 19:21:59 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include <fstream>

class Location;
class Server;

enum   state
{
    SERVER,
    LOCATION,
    NONE
};

// Static class
class ConfigParser 
{
private:
    static std::vector<Server*> servers;
    ConfigParser();
    ~ConfigParser();
public:
    static void                 parseConfigFile(std::string const &);
    static Server               *parseServer(std::ifstream &, std::string &, int &,  std::stack<state> &);
    static Location             *parseLocation(std::ifstream &, std::string &, int &, std::stack<state> &);
    static std::string          findLocation(std::string, int);
    static std::vector<Server*>  getServers();
};

