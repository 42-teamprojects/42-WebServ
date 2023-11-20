/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htalhaou <htalhaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 17:18:58 by htalhaou          #+#    #+#             */
/*   Updated: 2023/11/20 17:34:27 by htalhaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"


Cgi::Cgi()
{
	this->path = "";
	this->filename = "";
}

Cgi::Cgi(std::string path, std::string filename)
{
	this->path = path;
	this->filename = filename;
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

void Cgi::executCgi(Request &req, Response &res, Server &serv)
{
	std::string cgiPath;
	int fd[2];
	pipe(fd);
	pid_t pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], 1);
		dup2(fd[1], 2);
		close(fd[1]);
		cgiPath = this->path + this->filename;
		char *argv[] = {const_cast<char *>(cgiPath.c_str()), NULL};
		execve(cgiPath.c_str(), argv, NULL);
		exit(0);
	}
	else if (pid > 0)
	{
		close(fd[1]);
		waitpid(pid, NULL, 0);
		char buffer[1024];
		std::string body;
		int ret;
		while ((ret = recv(fd[0], buffer, 1023, 0)) > 0)
		{
			buffer[ret] = '\0';
			body += buffer;
		}
		close(fd[0]);
		res.setBody(body);
	}
	else
	{
		std::cout << "fork error" << std::endl;
	}
}
