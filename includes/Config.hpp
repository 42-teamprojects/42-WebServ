/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                   :+:      :+:    :+:   */
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
class Server;

enum state
{
    SERVER,
    ROUTE,
    NONE
};

// Static class
class Config
{
private:
    static std::vector<Server> servers;
    Config();
    ~Config();

public:
    static void                         parse(std::string const &);
    static Server                       parseServer(std::ifstream &, std::string &, int &, std::stack<state> &);
    static Route                        parseRoute(std::ifstream &, std::string &, int &, std::stack<state> &);
    static std::string                  findRoute(std::string, int);
    static std::vector<Server>          getServers();

    typedef std::vector<Server>::iterator iterator;

    static iterator begin() { return servers.begin(); }
    static iterator end() { return servers.end(); }

    // Helpers
    static iterator find(const std::string& host, int port)
    {
        for (iterator it = servers.begin(); it != servers.end(); ++it) {
            if (it->getHost() == host && it->getPort() == port) {
                return it;
            }
        }
        return servers.end();
    }
};
