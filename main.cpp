/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 22:27:57 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/08 19:29:59 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main() 
{
    try {
        ConfigParser::parseConfigFile("configs/default");
    } catch (std::exception &e) {
        std::cerr << "Config file: " << e.what() << std::endl;
    }

    std::vector<Server *> servers = ConfigParser::getServers(); 
    Server *server = servers[0];
    // std::cout << "listen: " << server.getListen() << std::endl;
    // std::cout << "host: " << server.getHost() << std::endl;
    // std::cout << "server_name: ";
    // for (size_t i = 0; i < server.getServerName().size(); i++)
    //     std::cout << server.getServerName()[i] << " ";
    // std::cout << std::endl;
    server->print(); 
    // for (size_t i = 0; i < servers.size(); i++)
        // servers[i].print();
    return 0;
}