/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htalhaou <htalhaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 22:27:57 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/15 15:34:00 by htalhaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main(int argc, char **argv) 
{
    std::string configPath;
    if (argc != 2)
        configPath = "configs/default";
    else
        configPath = argv[1];

    try {
        Config::parse(configPath);
        std::vector<Server> servers = Config::getServers(); 

        for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); ++it)
            it->print();
    } catch (std::exception &e) {
        std::cerr << "Config file: " << e.what() << std::endl;
    }

    WebServer server;
	server.run();
    return 0;
}