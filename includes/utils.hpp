/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 11:27:18 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/17 11:27:26 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
void trim(Container & s)
{
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    if (start == std::string::npos || end == std::string::npos)
        s.clear();
    else
        s = s.substr(start, end - start + 1);
}

template <typename Container>
Container ft_split(const std::string &s, const std::string &delimiters)
{
    Container splited;
    size_t i = 0;

    while (i < s.length())
    {
        i = s.find_first_not_of(delimiters, i);
        
        if (i == std::string::npos)
            break;
        size_t wordEnd = s.find_first_of(delimiters, i);
        
        std::string word = s.substr(i, wordEnd - i);
        trim(word);
        splited.push_back(word);
        
        i = wordEnd;
    }
    return splited;
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
