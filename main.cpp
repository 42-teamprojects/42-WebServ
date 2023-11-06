/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 22:27:57 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/06 20:02:29 by yelaissa         ###   ########.fr       */
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

    std::vector<Server> servers = ConfigParser::getServers(); 
    for (size_t i = 0; i < servers.size(); i++)
        servers[i].print();
    return 0;
}