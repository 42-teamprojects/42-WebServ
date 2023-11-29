/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 22:27:57 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/29 11:02:26 by yelaissa         ###   ########.fr       */
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

        Console::log(INFO, "Starting server");
        WebServer server(servers);
        server.run();
        
    } catch (ServerException &e) {
        Console::error("Config file: " + std::string(e.what()));
    }

    return 0;
}