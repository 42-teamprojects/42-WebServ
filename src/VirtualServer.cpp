/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 22:16:34 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/12 18:23:08 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "VirtualServer.hpp"
#include "webserv.hpp"

VirtualServer::VirtualServer() : port(-1), host(""), serverNames(), clientMaxBodySize(0), root(""), index(), routes() {};
VirtualServer::~VirtualServer() {};

// Getters
int                         VirtualServer::getPort() const { return port; }
std::string                 VirtualServer::getHost() const { return host; }
std::vector<std::string>    VirtualServer::getServerNames() const { return serverNames; }
size_t                      VirtualServer::getClientMaxBodySize() const { return clientMaxBodySize; }
std::string                 VirtualServer::getRoot() const { return root; }
std::vector<std::string>    VirtualServer::getIndex() const { return index; }
std::vector<Route>          VirtualServer::getRoutes() const { return (this->routes); }

// Setters
void                        VirtualServer::setPort(const int & port) { this->port = port; }
void                        VirtualServer::setHost(const std::string & host) { this->host = host; }
void                        VirtualServer::setServerNames(const std::vector<std::string>& serverNames) { this->serverNames = serverNames; }
void                        VirtualServer::setClientMaxBodySize(const size_t & clientMaxBodySize) { this->clientMaxBodySize = clientMaxBodySize; }
void                        VirtualServer::setRoot(const std::string & root) { this->root = root; }
void                        VirtualServer::setIndex(const std::vector<std::string>& index) { this->index = index; }
void                        VirtualServer::addRoute(Route route) { this->routes.push_back(route); }

// Methods
void                        VirtualServer::print() const
{
    if (port > 0)
        std::cout << "port: " << port << std::endl;
    if (!host.empty())
        std::cout << "host: " << host << std::endl;
    if (!serverNames.empty()) {
        std::cout << "server_names: ";
        printContainer(serverNames);
    }
    if (clientMaxBodySize > 0)
        std::cout << "client_max_body_size: " << clientMaxBodySize << std::endl;
    if (!root.empty())
        std::cout << "root: " << root << std::endl;
    if (!index.empty()) {
        std::cout << "index: ";
        printContainer(index);
    }
    if (!routes.empty()) {
        std::cout << "routes: " << std::endl;
        for (std::vector<Route>::const_iterator it = routes.begin(); it != routes.end(); ++it)
            (*it).print();
    }
}

void                        VirtualServer::fill(std::string const &line, int &lineNb)
{
    std::vector<std::string>    split;
    std::string                 option, value;

    split = ft_split<std::vector<std::string> >(line, "=");

    if (split.size() != 2)
        throw ServerException("Invalid server line", lineNb);
    option = split[0];
    value = split[1];

    if (value.empty() || option.empty())
        throw ServerException("Invalid server line", lineNb);
    if (option == "port") {
        char *end;
        size_t port = std::strtod(value.c_str(), &end);
        if (*end != '\0')
            throw ServerException("Invalid server line", lineNb); 
        setPort(port);
    }
    else if (option == "host") {
        setHost(value);
    }
    else if (option == "server_names") {
        std::vector<std::string>    names = ft_split<std::vector<std::string> >(value, ", ");
        if (names.empty())
            throw ServerException("Invalid server line", lineNb);
        setServerNames(names);
    }
    else if (option == "client_max_body_size") {
        char *end;
        double size = std::strtod(value.c_str(), &end);
        if (*end != '\0')
            throw ServerException("Invalid body size", lineNb);   
        setClientMaxBodySize(size);
    }
    else if (option == "root") {
        setRoot(value);
    }
    else if (option == "index") {
        std::vector<std::string>    index = ft_split<std::vector<std::string> >(value, ", ");
        if (index.empty())
            throw ServerException("Invalid server line", lineNb);
        setIndex(index);
    }
    else
        std::cout << "Invalid server option: " << line << std::endl;
        // throw ServerException("Invalid server option", lineNb);
}