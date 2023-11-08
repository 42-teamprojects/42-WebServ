/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 22:42:11 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/08 15:52:53 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "webserv.hpp"

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
    if (path.empty())
        std::cout << "path: " << path << std::endl;
    if (redir.empty())
        std::cout << "return: " << redir << std::endl;
    printCommon();
}

void                        Location::fill(std::string const &line, int &lineNb)
{
    std::vector<std::string>    split;
    std::string                 option, value;

    split = ft_split<std::vector<std::string> >(line, "=");

    std::string commonOptions[] = {"error_page", "client_max_body_size", "autoindex", "root", "index", "allow_methods"};
    
    if (split.size() != 2)
        throw ServerException("Invalid server line", lineNb);
    option = split[0];
    value = split[1];
    if (option == "redir") {
        setRedir(value);
    }
    else if (std::find(commonOptions, commonOptions + 6, option) != commonOptions + 6)
        fillCommon(option, value, lineNb);
    else
        throw ServerException("Invalid server option", 0);
}