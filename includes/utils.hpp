/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msodor <msodor@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 11:27:18 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/28 23:35:36 by msodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
typedef enum e_HttpStatusCode {
    OK = 200,
    MovedPermanently = 301,
    BadRequest = 400,
    Unauthorized = 401,
    Forbidden = 403,
    NotFound = 404,
    MethodNotAllowed = 405,
    RequestURITooLong = 414,
    RequestEntityTooLarge = 413,
    NotImplemented = 501,
    ServerError = 500
} HttpStatusCode;

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


