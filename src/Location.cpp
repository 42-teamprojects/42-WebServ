/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 22:42:11 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/06 19:58:23 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() {};
Location::~Location() {};

// Getters
std::string                 Location::getPath() const { return path; }
std::string                 Location::getRedir() const { return redir; }

// Setters
void                        Location::setPath(const std::string& path) { this->path = path; }
void                        Location::setRedir(const std::string& redir) { this->redir = redir; }

// Methods
void                        Location::print() const
{
    std::cout << "path: " << path << std::endl;
    std::cout << "return: " << redir << std::endl;
    std::cout << std::endl;
    std::cout << "error_page: ";
    for (size_t i = 0; i < errorPage.size(); i++)
        std::cout << errorPage[i] << " ";
    std::cout << std::endl;
    std::cout << "client_max_body_size: " << clientMaxBodySize << std::endl;
    std::cout << "autoindex: " << autoIndex << std::endl;
    std::cout << "root: " << root << std::endl;
    std::cout << "index: ";
    for (size_t i = 0; i < index.size(); i++)
        std::cout << index[i] << " ";
    std::cout << std::endl;
    std::cout << "allow_methods: ";
    for (size_t i = 0; i < allowMethods.size(); i++)
        std::cout << allowMethods[i] << " ";
    std::cout << std::endl;
    std::cout << "locations: " << std::endl;
    for (size_t i = 0; i < locations.size(); i++)
        locations[i].print();
    std::cout << std::endl;
}

void                        Location::fill(std::string const &line, int &lineNb)
{
    std::vector<std::string>    split;
    std::string                 option, value;

    split = ft_split<std::vector<std::string> >(line, "=");
    
    if (split.size() != 2)
        throw ServerException("Invalid server line", lineNb);
    option = split[0];
    value = split[1];
    if (option == "redir") {
        setRedir(value);
    }
    else if (option == "error_page") {
        std::vector<std::string>    errors = ft_split<std::vector<std::string> >(value, " ");
        
        if (errors.size() < 2)
            throw ServerException("Invalid server line", lineNb);
        if (errors[0] != "400" && errors[0] != "401" && errors[0] != "403" && errors[0] != "404" && errors[0] != "405" && errors[0] != "413" && errors[0] != "500" && errors[0] != "501" && errors[0] != "505")
            throw ServerException("Invalid server line", lineNb);
        setErrorPage(errors);
    }
    else if (option == "client_max_body_size") {
        char *end;
        size_t size = std::strtod(value.c_str(), &end);
        if (*end != '\0')
            throw ServerException("Invalid server line", lineNb);   
        setClientMaxBodySize(size);
    }
    else if (option == "autoindex")
        setAutoIndex((value == "on" || value == "true") ? true : false);
    else if (option == "allow_methods") {
        std::vector<std::string>    methods = ft_split<std::vector<std::string> >(value, " ");
        
        if (methods.size() < 1)
            throw ServerException("Invalid server line", lineNb);
        for (size_t i = 0; i < methods.size(); i++) {
            if (methods[i] != "GET" && methods[i] != "HEAD" && methods[i] != "POST" && methods[i] != "PUT" && methods[i] != "DELETE" && methods[i] != "CONNECT" && methods[i] != "OPTIONS" && methods[i] != "TRACE")
                throw ServerException("Invalid server line", lineNb);
        }
        setAllowMethods(methods);
    }
    else if (option == "root")
        setRoot(value);
    else if (option == "index") {
        std::vector<std::string>    indexes = ft_split<std::vector<std::string> >(value, " ");
    
        if (index.size() < 1)
            throw ServerException("Invalid server line", lineNb);
        setIndex(indexes);
    }
    else
        throw ServerException("Invalid server option", 0);
}