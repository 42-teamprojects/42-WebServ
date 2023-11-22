/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerException.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 21:39:01 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/22 13:15:16 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerException.hpp"

ServerException::ServerException(std::string const &msg) throw() : msg(msg)
{
}

ServerException::ServerException(std::string const &msg, int line) throw() : msg(msg)
{
    std::stringstream ss;
    ss << " at line " << line;
    this->msg += ss.str();
}

ServerException::ServerException(HttpStatusCode code) throw() : code(code)
{
}

ServerException::~ServerException() throw()
{
}

const char *ServerException::what() const throw()
{
    return (this->msg.c_str());
}

HttpStatusCode ServerException::getCode() const throw()
{
    return (this->code);
}