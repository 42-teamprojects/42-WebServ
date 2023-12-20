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
		this->total_read = src.total_read;
		this->response = src.response;
		this->total_send = src.total_send;
		this->method = src.method;
		this->chunked = src.chunked;
		this->startCunter = src.startCunter;
		this->content_length = src.content_length;

	}
	return (*this);
}

Client::Client(int socket)
{
	this->socket = socket;
	this->buffer = "";
	this->total_read = 0;
	this->total_send = 0;
	this->response = "";
	this->method = "";
	this->chunked = false;
	this->startCunter = 0;
	this->content_length = 0;
	
}

int Client::getSocket() const
{
	return (socket);
}

std::string Client::getBuffer() const
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

void Client::add_to_total_read(size_t read)
{
	total_read += read;
}

size_t Client::getTotalRead() const
{
	return (total_read);
}

Client::Client(std::string response, int read)
{
	this->buffer = response;
	this->total_read = read;
}
