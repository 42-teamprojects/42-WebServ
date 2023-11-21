/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 22:16:34 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/21 21:46:43 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "webserv.hpp"

Server::Server() : port(-1), host(""), serverNames(), clientMaxBodySize(0), root(DEFAULT_ROOT), index(), allowListing(false), routes() {};
Server::~Server() {};

// Getters
int                         Server::getPort() const { return port; }
std::string                 Server::getHost() const { return host; }
std::vector<std::string>    Server::getServerNames() const { return serverNames; }
size_t                      Server::getClientMaxBodySize() const { return clientMaxBodySize; }
std::string                 Server::getRoot() const { return root; }
std::vector<std::string>    Server::getIndex() const { return index; }
bool                        Server::getAllowListing() const { return allowListing;}
std::vector<std::string>    Server::getErrorPages() const { return errorPages; }
std::vector<Route>          Server::getRoutes() const { return (this->routes); }

// Setters
void                        Server::setPort(const int & port) { this->port = port; }
void                        Server::setHost(const std::string & host) { this->host = host; }
void                        Server::setServerNames(const std::vector<std::string>& serverNames) { this->serverNames = serverNames; }
void                        Server::setClientMaxBodySize(const size_t & clientMaxBodySize) { this->clientMaxBodySize = clientMaxBodySize; }
void                        Server::setRoot(const std::string & root) { this->root = root; }
void                        Server::setAllowListing(const bool & allowListing) { this->allowListing = allowListing; }
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
    std::cout << "allow_listing: " << allowListing << std::endl;
    if (!routes.empty()) {
        std::cout << "routes: " << std::endl;
        for (std::vector<Route>::const_iterator it = routes.begin(); it != routes.end(); ++it)
            (*it).print();
    }
}

void Server::fill(std::string const &line, int &lineNb)
{
    std::vector<std::string>    split;
    std::string                 option, value;

    split = ft_split(line, "=");

    if (split.size() != 2)
        throw ServerException("Invalid server line", lineNb);
    option = split[0];
    value = split[1];

    if (value.empty() || option.empty())
        throw ServerException("Invalid server line", lineNb);
    if (option == "port" && port == -1) {
        char *end;
        size_t port = std::strtod(value.c_str(), &end);
        if (*end != '\0')
            throw ServerException("Invalid server line", lineNb); 
        setPort(port);
    }
    else if (option == "host" && host.empty()) {
        setHost(value);
    }
    else if (option == "server_names" && serverNames.empty()) {
        std::vector<std::string>    names = ft_split(value, ", ");
        if (names.empty())
            throw ServerException("Invalid server line", lineNb);
        setServerNames(names);
    }
    else if (option == "client_max_body_size" && clientMaxBodySize == 0) {
        char *end;
        double size = std::strtod(value.c_str(), &end);
        if (*end != '\0')
            throw ServerException("Invalid body size", lineNb);   
        setClientMaxBodySize(size);
    }
    else if (option == "root" && root == DEFAULT_ROOT) {
        setRoot(value);
    }
    else if (option == "index" && index.empty()) {
        std::vector<std::string>    index = ft_split(value, ", ");
        if (index.empty())
            throw ServerException("Invalid server line", lineNb);
        setIndex(index);
    }
    else if (option == "error_pages" && errorPages.empty()) {
        std::vector<std::string>    pages = ft_split(value, ", ");
        if (pages.empty())
            throw ServerException("Invalid server line", lineNb);
        setErrorPages(pages);
    }
    else if (option == "allow_listing" && allowListing == false) {
        if (value == "on" || value == "1" || value == "true")
            setAllowListing(true);
        else if (value == "off" || value == "0" || value == "false")
            setAllowListing(false);
        else
            throw ServerException("Invalid server line", lineNb);
    }
    else
        Console::warning("Invalid server option: " + line);
        // throw ServerException("Invalid server option", lineNb);
}