/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htalhaou <htalhaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 13:25:18 by htalhaou          #+#    #+#             */
/*   Updated: 2023/12/10 16:32:32 by htalhaou         ###   ########.fr       */
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
		this->socket = src.socket;
		this->buffer = src.buffer;
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
