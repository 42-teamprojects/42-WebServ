/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 22:27:57 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/12 18:28:51 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main() 
{
    try {
        ConfigParser::parseConfigFile("configs/default");
        std::vector<VirtualServer> servers = ConfigParser::getVirtualServers(); 
        VirtualServer server = servers[0];
        server.print(); 
    } catch (std::exception &e) {
        std::cerr << "Config file: " << e.what() << std::endl;
    }

    return 0;
}