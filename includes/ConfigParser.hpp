/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 11:03:20 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/12 18:28:51 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include <fstream>

class Route;
class VirtualServer;

enum   state
{
    SERVER,
    ROUTE,
    NONE
};

// Static class
class ConfigParser 
{
private:
    static std::vector<VirtualServer> servers;
    ConfigParser();
    ~ConfigParser();
public:
    static void                 parseConfigFile(std::string const &);
    static VirtualServer               parseVirtualServer(std::ifstream &, std::string &, int &,  std::stack<state> &);
    static Route                parseRoute(std::ifstream &, std::string &, int &, std::stack<state> &);
    static std::string          findRoute(std::string, int);
    static std::vector<VirtualServer>  getVirtualServers();
};

