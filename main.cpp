/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 22:27:57 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/19 18:09:05 by yelaissa         ###   ########.fr       */
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

        WebServer server(servers);
        server.run();
        
        // for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); ++it)
        //     it->print();
    } catch (ServerException &e) {
        std::cerr << "Config file: " << e.what() << std::endl;
    }

    return 0;
}