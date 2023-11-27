/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 17:18:58 by htalhaou          #+#    #+#             */
/*   Updated: 2023/11/27 17:45:42 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

Cgi::Cgi()
{
}

Cgi::Cgi(std::string path, std::string filename)
{
	this->path = path;
	this->filename = filename;
	executCgi();
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

std::string Cgi::getResponseBody()
{
	return (this->responseBody);
}

void Cgi::executCgi()
{
	std::string cgiPath;
	int fd[2];
	pipe(fd);
	pid_t pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], 1);
		char *argv[] = {const_cast<char *>(this->path.c_str()), const_cast<char *>(filename.c_str()), NULL};
		execve(this->path.c_str(), argv, NULL);
		Console::error("execve failed");
		close(fd[1]);
		exit(1);
	}
	else if (pid > 0)
	{
		close(fd[1]);
		waitpid(pid, NULL, 0);
		char buffer[1024];
		std::string body;
		int ret;
		while ((ret = read(fd[0], buffer, 1023)) > 0)
		{
			buffer[ret] = '\0';
			body += buffer;
		}
		this->responseBody = body;
		close(fd[0]);
		return;
	}
	else
		Console::error("fork failed");
	throw ServerException(ServerError);
}