/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mimes.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 10:12:46 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/29 10:52:47 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mimes.hpp"
#include "utils.hpp"

// std::vector<std::string> ft_split(const std::string &s, const std::string &delimiters);

Mimes::Mimes()
{
    std::ifstream file("mimes.txt");
    std::string line;
    std::vector<std::string> pairs;
    
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line[0] == '#')
                continue;
            
            pairs = ft_split(line, "|");
            if (pairs.size() != 2)
                continue;
            std::vector<std::string> exts = ft_split(pairs[0], ",");
            mimes[exts] = pairs[1];
            pairs.clear();
        }
        file.close();
    }
}

Mimes::~Mimes()
{
}

std::string Mimes::operator[](std::string const &key)
{
    std::map<std::vector<std::string>, std::string>::iterator it;
    for (it = mimes.begin(); it != mimes.end(); it++)
    {
        if (std::find(it->first.begin(), it->first.end(), key) != it->first.end())
            return (it->second);
    }
    return ("text/plain");
}

// void Mimes::print()
// {
//     std::map<std::vector<std::string>, std::string>::iterator it;
//     for (it = mimes.begin(); it != mimes.end(); it++)
//     {
//         std::cout << it->second << " : ";
//         for (std::vector<std::string>::iterator it2 = it->first.begin(); it2 != it->first.end(); it2++)
//         {
//             std::cout << *it2 << " ";
//         }
//         std::cout << std::endl;
//     }
// }
