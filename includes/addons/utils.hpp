/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 11:27:18 by yelaissa          #+#    #+#             */
/*   Updated: 2023/12/06 18:08:09 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

template <typename T>
std::string toString(T value)
{
    std::ostringstream os;
    os << value;
    return os.str();
}

template <typename Container>
void printContainer(const Container& c)
{
    typename Container::const_iterator it;
    for (it = c.begin(); it != c.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

template <typename Container>
void printMap(const Container& c)
{
    typename Container::const_iterator it;
    for (it = c.begin(); it != c.end(); ++it)
    {
        std::cout << it->first << " : " << it->second << std::endl;    
    }
    std::cout << std::endl;
}

std::vector<std::string>    getFilesInDirectory(std::string const &, std::string const &);
std::string                 generateHtmlListing(const std::vector<std::string>&);
void                        removeConsecutiveChars(std::string &, char);
void                        trim(std::string &);
std::vector<std::string>    ft_split(const std::string &, const std::string &);
bool                        mapErrorPages(std::map<int, std::string> & errorPages, std::string const & value);
bool                        isDirectory(std::string);
bool                        isFile(std::string);
std::pair<std::string, bool>                   getMatchedPath(std::string serverRootPath, std::string path);
void                        trimTrailingSlashes(std::string & s);
std::vector<std::string>    split(const std::string& s, const std::string& delimiter);
bool                        mapCgi(std::map<std::string, std::string> & cgi, std::string const & value);

