/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 11:44:08 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/30 13:15:58 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "helpers.hpp"
#include <sys/stat.h>
#include <sys/types.h>
#include <ctime>
#include <sstream>
#include <iomanip>

std::string getDateGMT()
{
    std::time_t t = std::time(0);
    std::tm tm = *std::gmtime(&t);
    std::stringstream ss;
    ss << std::put_time(&tm, "%a, %d %b %Y %H:%M:%S GMT");
    return (ss.str());
}

std::string getFileExt(std::string const &path)
{
    std::string ext = path.substr(path.find_last_of("."));
    return (ext);
}

bool hasAccess(std::string const &path)
{
    struct stat st;
    if (stat(path.c_str(), &st) == 0)
        return (true);
    return (false);
}

std::string getRemoteAddress()
{
    std::string remoteAddress = getenv("REMOTE_ADDR");
    return (remoteAddress);
}