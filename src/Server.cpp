/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 22:16:34 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/13 22:00:10 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "webserv.hpp"

Server::Server() : port(-1), host(""), serverNames(), clientMaxBodySize(0), root(""), index(), routes() {};
Server::~Server() {};

// Getters
int                         Server::getPort() const { return port; }
std::string                 Server::getHost() const { return host; }
std::vector<std::string>    Server::getServerNames() const { return serverNames; }
size_t                      Server::getClientMaxBodySize() const { return clientMaxBodySize; }
std::string                 Server::getRoot() const { return root; }
std::vector<std::string>    Server::getIndex() const { return index; }
std::vector<std::string>    Server::getErrorPages() const { return errorPages; }
std::vector<Route>          Server::getRoutes() const { return (this->routes); }

// Setters
void                        Server::setPort(const int & port) { this->port = port; }
void                        Server::setHost(const std::string & host) { this->host = host; }
void                        Server::setServerNames(const std::vector<std::string>& serverNames) { this->serverNames = serverNames; }
void                        Server::setClientMaxBodySize(const size_t & clientMaxBodySize) { this->clientMaxBodySize = clientMaxBodySize; }
void                        Server::setRoot(const std::string & root) { this->root = root; }
void                        Server::setErrorPages(const std::vector<std::string>& errorPages) { this->errorPages = errorPages; }
void                        Server::setIndex(const std::vector<std::string>& index) { this->index = index; }
void                        Server::addRoute(Route route) { this->routes.push_back(route); }

// Methods
void                        Server::print() const
{
    std::cout << "--------------" << std::endl;
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
    if (!errorPages.empty()) {
        std::cout << "error_pages: ";
        printContainer(errorPages);
    }
    if (!routes.empty()) {
        std::cout << "routes: " << std::endl;
        for (std::vector<Route>::const_iterator it = routes.begin(); it != routes.end(); ++it)
            (*it).print();
    }
}

void                        Server::fill(std::string const &line, int &lineNb)
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
    else if (option == "error_pages") {
        std::vector<std::string>    pages = ft_split<std::vector<std::string> >(value, ", ");
        if (pages.empty())
            throw ServerException("Invalid server line", lineNb);
        setErrorPages(pages);
    }
    else
        std::cout << "Invalid server option: " << line << std::endl;
        // throw ServerException("Invalid server option", lineNb);
}