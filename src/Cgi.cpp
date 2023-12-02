/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 17:18:58 by htalhaou          #+#    #+#             */
/*   Updated: 2023/12/01 10:57:09 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"
#include "enums.hpp"

Cgi::Cgi()
{
}

static std::string getQuery(std::string const & uri)
{
	std::string query;
	size_t pos = uri.find("?");
	if (pos != std::string::npos)
		query = uri.substr(pos + 1);
	return (query);
}

static char **mapToArray(std::map<std::string, std::string> const & map)
{
	char **array = new char*[map.size() + 1];
	int i = 0;
	for (std::map<std::string, std::string>::const_iterator it = map.begin(); it != map.end(); it++)
	{
		std::string str = it->first + "=" + it->second;
		array[i] = new char[str.size() + 1];
		strcpy(array[i], str.c_str());
		i++;
	}
	array[i] = NULL;
	return (array);
}

static std::map<std::string, std::string> getEnv(Request const & req, std::string const & filename)
{
	std::map<std::string, std::string> env;
	env["SERVER_SOFTWARE"] = "webserv/1.0";
	env["GATEWAY_INTERFACE"] = "CGI/1.1";
	env["SERVER_PROTOCOL"] = req.getVersion();
	env["SERVER_PORT"] = toString(req.getPort());
	env["REQUEST_METHOD"] = req.getMethod();
	env["PATH_INFO"] = filename;
	env["PATH_TRANSLATED"] = filename;
	env["SCRIPT_NAME"] = req.getUri();
	env["QUERY_STRING"] = getQuery(req.getUri());
	env["REMOTE_HOST"] = req.getHost();
	env["CONTENT_LENGTH"] = req.getContentLength() < 0 ? "" : toString(req.getContentLength());
	env["CONTENT_TYPE"] = req.getContentType();
	env["HTTP_ACCEPT"] = req.getHeaders().find("Accept")->second;
	env["HTTP_USER_AGENT"] = req.getHeaders().find("User-Agent")->second;
	return (env);
}

Cgi::Cgi(std::string const & path, std::string const & filename, Request const & req)
{
	this->path = path;
	this->filename = filename;
	this->env = getEnv(req, filename);
	this->envp = mapToArray(this->env);
	executCgi();
}

Cgi::Cgi(Cgi const& other)
{
	*this = other;
}


Cgi::~Cgi()
{
	for (int i = 0; this->envp[i]; i++)
		delete [] this->envp[i];
	delete [] this->envp;
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
	int status;
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], 1);
		char *argv[] = {const_cast<char *>(this->path.c_str()), const_cast<char *>(filename.c_str()), NULL};
		execve(this->path.c_str(), argv, envp);
		close(fd[1]);
		exit(1);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WEXITSTATUS(status) != 0)
		{
			Console::error("execve failed");
			throw ServerException(BadGateway);
		}
		close(fd[1]);
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
