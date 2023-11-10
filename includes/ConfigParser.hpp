/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 11:03:20 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/10 20:08:02 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include <fstream>

class Route;
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
    static Route             *parseRoute(std::ifstream &, std::string &, int &, std::stack<state> &);
    static std::string          findRoute(std::string, int);
    static std::vector<Server*>  getServers();
};

