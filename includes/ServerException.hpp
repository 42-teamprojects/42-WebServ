/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerException.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 21:33:59 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/05 21:44:03 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>
#include <string>
#include <iostream>
#include <sstream>

class ServerException : public std::exception
{
private:
    std::string msg;
public:
    ServerException(std::string const &msg) throw();
    ServerException(std::string const &msg, int line) throw();
    virtual ~ServerException() throw();
    virtual const char *what() const throw();
};
