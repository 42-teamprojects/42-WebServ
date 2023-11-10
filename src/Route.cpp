/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 22:42:11 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/08 20:01:36 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Route.hpp"
#include "webserv.hpp"

Route::Route() : Context(), path(""), redir("") {};
Route::~Route() {};

// Getters
std::string                 Route::getPath() const { return path; }
std::string                 Route::getRedir() const { return redir; }

// Setters
void                        Route::setPath(const std::string& path) { this->path = path; }
void                        Route::setRedir(const std::string& redir) { this->redir = redir; }

// Methods
void                        Route::print() const
{
    if (!path.empty())
        std::cout << "path: " << path << std::endl;
    if (!redir.empty())
        std::cout << "return: " << redir << std::endl;
    printCommon();
}

void                        Route::fill(std::string const &line, int &lineNb)
{
    std::vector<std::string>    split;
    std::string                 option, value;

    split = ft_split<std::vector<std::string> >(line, "=");

    std::string commonOptions[] = {"error_page", "client_max_body_size", "autoindex", "root", "index", "allow_methods"};
    
    if (split.size() != 2)
        throw ServerException("Invalid server line", lineNb);
    option = split[0];
    value = split[1];
    if (option == "return") {
        setRedir(value);
    }
    else if (std::find(commonOptions, commonOptions + 6, option) != commonOptions + 6)
        fillCommon(option, value, lineNb);
    else
        std::cout << "Invalid route option: " << line << std::endl;
        // throw ServerException("Invalid server option", lineNb);
}