/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htalhaou <htalhaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 13:25:18 by htalhaou          #+#    #+#             */
/*   Updated: 2023/12/10 14:06:53 by htalhaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "WebServer.hpp"
#include "webserv.hpp"

Client::Client()
{
}

Client::~Client()
{
}

Client::Client(Client const& src)
{
	*this = src;
}

Client &Client::operator=(Client const& src)
{
	if (this != &src)
	{
	}
	return (*this);
}

Client::Client(int socket) : socket(socket)
{
}

int Client::getSocket() const
{
	return (socket);
}

std::string Client::getBuffer()
{
	return (buffer);
}

void Client::setBuffer(std::string buffer)
{
	this->buffer = buffer;
}

void Client::setSocket(int socket)
{
	this->socket = socket;
}
