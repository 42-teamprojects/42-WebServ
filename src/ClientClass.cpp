/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientClass.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htalhaou <htalhaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 13:25:18 by htalhaou          #+#    #+#             */
/*   Updated: 2023/12/10 14:06:53 by htalhaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "WebServer.hpp"
#include "webserv.hpp"

ClientClass::ClientClass()
{
}

ClientClass::~ClientClass()
{
}

ClientClass::ClientClass(ClientClass const& src)
{
	*this = src;
}

ClientClass &ClientClass::operator=(ClientClass const& src)
{
	if (this != &src)
	{
	}
	return (*this);
}

ClientClass::ClientClass(int socket) : socket(socket)
{
}

int ClientClass::getSocket() const
{
	return (socket);
}

std::string ClientClass::getBuffer()
{
	return (buffer);
}

void ClientClass::setBuffer(std::string buffer)
{
	this->buffer = buffer;
}

void ClientClass::setSocket(int socket)
{
	this->socket = socket;
}
