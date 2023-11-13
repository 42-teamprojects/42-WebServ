/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 22:27:57 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/13 21:59:21 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main() 
{
    try {
        Config::parse("configs/default");
        std::vector<Server> servers = Config::getServers(); 

        for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); ++it)
            it->print();
    } catch (std::exception &e) {
        std::cerr << "Config file: " << e.what() << std::endl;
    }

    return 0;
}