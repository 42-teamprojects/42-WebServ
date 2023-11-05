/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 22:16:34 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/05 22:50:00 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() {};
Server::~Server() {};

// Getters
std::string                 Server::getListen() const { return listen; }
std::string                 Server::getHost() const { return host; }
std::vector<std::string>    Server::getServerName() const { return serverName; }

// Setters
void                        Server::setListen(const std::string& listen) { this->listen = listen; }
void                        Server::setHost(const std::string& host) { this->host = host; }
void                        Server::setServerName(const std::vector<std::string>& serverName) { this->serverName = serverName; }

// Methods
void                        Server::print() const
{

}
void                        Server::fill(std::string const &line, int &lineNb)
{
    (void) line;
    (void) lineNb; 
}