/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Context.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 13:24:34 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/08 15:52:18 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Context.hpp"
#include "webserv.hpp"

Context::Context()
{
}

Context::~Context()
{
}

// Getters
std::vector<std::string>    Context::getErrorPage() const
{
    return (this->errorPage);
}

size_t                      Context::getClientMaxBodySize() const
{
    return (this->clientMaxBodySize);
}

std::string                 Context::getRoot() const
{
    return (this->root);
}

std::vector<std::string>    Context::getIndex() const
{
    return (this->index);
}

bool                        Context::getAutoIndex() const
{
    return (this->autoIndex);
}

std::vector<std::string>    Context::getAllowMethods() const
{
    return (this->allowMethods);
}

std::vector<Location*>       Context::getLocations() const
{
    return (this->locations);
}

// Setters
void                        Context::setErrorPage(const std::vector<std::string>& errorPage)
{
    this->errorPage = errorPage;
}

void                        Context::setClientMaxBodySize(size_t clientMaxBodySize)
{
    this->clientMaxBodySize = clientMaxBodySize;
}

void                        Context::setRoot(const std::string& root)
{
    this->root = root;
}

void                        Context::setIndex(const std::vector<std::string>& index)
{
    this->index = index;
}

void                        Context::setAutoIndex(bool autoIndex)
{
    this->autoIndex = autoIndex;
}

void                        Context::setAllowMethods(const std::vector<std::string>& allowMethods)
{
    this->allowMethods = allowMethods;
}

void                        Context::setLocations(const std::vector<Location*>& locations)
{
    this->locations = locations;
}

void                        Context::addLocation(Location& location)
{
    this->locations.push_back(&location);
}

// Methods
void Context::printCommon() const
{
    if (!this->errorPage.empty())
    {
        std::cout << "errorPage: ";
        for (size_t i = 0; i < this->errorPage.size(); i++)
            std::cout << this->errorPage[i] << " ";
        std::cout << std::endl;
    }
    if (this->clientMaxBodySize != 0)
        std::cout << "clientMaxBodySize: " << this->clientMaxBodySize << std::endl;
    if (!this->root.empty())
        std::cout << "root: " << this->root << std::endl;
    if (!this->index.empty())
    {
        std::cout << "index: ";
        for (size_t i = 0; i < this->index.size(); i++)
            std::cout << this->index[i] << " ";
        std::cout << std::endl;
    }
    std::cout << "autoIndex: " << this->autoIndex << std::endl;
    if (!this->allowMethods.empty())
    {
        std::cout << "allowMethods: ";
        for (size_t i = 0; i < this->allowMethods.size(); i++)
            std::cout << this->allowMethods[i] << " ";
        std::cout << std::endl;
    }
    if (!this->locations.empty())
    {
        std::cout << "locations: " << std::endl;
        for (size_t i = 0; i < this->locations.size(); i++)
            this->locations[i]->print();
    }
}

void                        Context::fillCommon(std::string & option, std::string & value, int & lineNb)
{
    if (option == "error_page") {
        std::vector<std::string>    errors = ft_split<std::vector<std::string> >(value, " ");
        
        if (errors.size() < 2)
            throw ServerException("Invalid server line", lineNb);
        std::string errorCodes[] = {"400", "401", "403", "404", "405", "413", "500", "501", "505"};
        if (std::find(errorCodes, errorCodes + 9, errors[0]) == errorCodes + 9)
            throw ServerException("Invalid error code", lineNb);
        setErrorPage(errors);
    }
    else if (option == "client_max_body_size") {
        char *end;
        double size = std::strtod(value.c_str(), &end);
        if (*end != '\0')
            throw ServerException("Invalid body size", lineNb);   
        setClientMaxBodySize(size);
    }
    else if (option == "autoindex")
        setAutoIndex((value == "on" || value == "true") ? true : false);
    else if (option == "allow_methods") {
        std::vector<std::string>    methods = ft_split<std::vector<std::string> >(value, " ");
        
        if (methods.size() < 1)
            throw ServerException("Invalid server line", lineNb);
        std::string methodsList[] = {"GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE"};
        for (size_t i = 0; i < methods.size(); i++) {
            if (std::find(methodsList, methodsList + 8, methods[i]) == methodsList + 8)
                throw ServerException("Invalid method", lineNb);
        }
        setAllowMethods(methods);
    }
    else if (option == "root")
        setRoot(value);
    else if (option == "index") {
        std::vector<std::string>    indexes = ft_split<std::vector<std::string> >(value, " ");
    
        if (indexes.size() < 1)
            throw ServerException("Invalid server line", lineNb);
        setIndex(indexes);
    }
    else
        throw ServerException("Invalid server option", 0);
}