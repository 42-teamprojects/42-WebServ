/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htalhaou <htalhaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 17:18:58 by htalhaou          #+#    #+#             */
/*   Updated: 2023/11/18 17:26:07 by htalhaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"


Cgi::Cgi()
{
	this->path = "";
	this->filename = "";
}

Cgi::Cgi(Cgi const& other)
{
	*this = other;
}


Cgi::~Cgi()
{
}
Cgi& Cgi::operator=(Cgi const& other)
{
	if (this != &other)
	{
		this->path = other.path;
		this->filename = other.filename;
	}
	return (*this);
}

void Cgi::setPath(std::string path)
{
	this->path = path;
}

void Cgi::setFilename(std::string filename)
{
	this->filename = filename;
}

std::string Cgi::getPath()
{
	return (this->path);
}

std::string Cgi::getFilename()
{
	return (this->filename);
}
