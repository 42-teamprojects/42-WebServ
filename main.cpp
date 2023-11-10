/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 22:27:57 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/10 21:38:57 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main() 
{
    try {
        ConfigParser::parseConfigFile("configs/default");
        std::vector<Server *> servers = ConfigParser::getServers(); 
        Server *server = servers[0];
        server->print(); 
    } catch (std::exception &e) {
        std::cerr << "Config file: " << e.what() << std::endl;
    }

    return 0;
}