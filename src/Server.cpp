/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 22:16:34 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/08 19:19:31 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "webserv.hpp"

Server::Server() : Context(), listen(-1), host(""), serverName() {};
Server::~Server() {};

// Getters
size_t                      Server::getListen() const { return listen; }
std::string                 Server::getHost() const { return host; }
std::vector<std::string>    Server::getServerName() const { return serverName; }

// Setters
void                        Server::setListen(const size_t & listen) { this->listen = listen; }
void                        Server::setHost(const std::string & host) { this->host = host; }
void                        Server::setServerName(const std::vector<std::string>& serverName) { this->serverName = serverName; }

// Methods
void                        Server::print() const
{
    if (!listen)
        std::cout << "listen: " << listen << std::endl;
    if (!host.empty())
        std::cout << "host: " << host << std::endl;
    if (!serverName.empty()) {
        std::cout << "server_name: ";
        for (size_t i = 0; i < serverName.size(); i++)
            std::cout << serverName[i] << " ";
        std::cout << std::endl;
    }
    printCommon();
}

void                        Server::fill(std::string const &line, int &lineNb)
{
    std::vector<std::string>    split;
    std::string                 option, value;

    split = ft_split<std::vector<std::string> >(line, "=");

    std::string commonOptions[] = {"error_page", "client_max_body_size", "autoindex", "root", "index", "allow_methods"};
    
    if (split.size() != 2)
        throw ServerException("Invalid server line", lineNb);
    option = split[0];
    value = split[1];
    if (value.empty() || option.empty())
        throw ServerException("Invalid server line", lineNb);
    if (option == "listen") {
        char *end;
        size_t port = std::strtod(value.c_str(), &end);
        if (*end != '\0')
            throw ServerException("Invalid server line", lineNb); 
        setListen(port);
    }
    else if (option == "host") {
        setHost(value);
    }
    else if (option == "server_name") {
        std::vector<std::string>    names = ft_split<std::vector<std::string> >(value, " ");
        if (names.empty())
            throw ServerException("Invalid server line", lineNb);
        setServerName(names);
    }
    else if (std::find(commonOptions, commonOptions + 6, option) != commonOptions + 6)
        fillCommon(option, value, lineNb);
    else
        throw ServerException("Invalid server option", 0);
}