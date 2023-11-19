/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 15:08:25 by htalhaou          #+#    #+#             */
/*   Updated: 2023/11/19 20:59:11 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"
#include "webserv.hpp"

WebServer::WebServer(std::vector<Server> &servers) : servers(servers)
{
	srvs.resize(servers.size() + 1);
	std::vector<Server>::iterator it = servers.begin();
	FD_ZERO(&master); 
	int i = 0;
	for (; it != servers.end(); ++it)
	{
		Logger::info("WebServ is listening on port \033[1;32m" + toString(it->getPort()) + "\033[0m");
		handle_select(it->getPort(), i++);
	}
}

WebServer::~WebServer()
{
	for (size_t i = 0; i < servers.size(); i++)
		close(srvs[i].socket);
}

void WebServer::handle_select(int port, int idx)
{
	srvs[idx].socket = socket(AF_INET, SOCK_STREAM, 0);
	if (srvs[idx].socket < 0)
	{
		std::cerr << "error: socket creation" << std::endl;
		exit(1);
	}
	if (fcntl(srvs[idx].socket, F_SETFL, O_NONBLOCK, O_CLOEXEC) == -1)
	{
		std::cerr << "error: fcntl() failed" << std::endl;
		close(srvs[idx].socket);
		exit(1);
	}
	FD_SET(srvs[idx].socket, &master);

	memset(&srvs[idx].addr, 0, sizeof(srvs[idx].addr));
	srvs[idx].addr.sin_family = AF_INET;
	srvs[idx].addr.sin_port = htons(port);
	srvs[idx].addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int yes = 1;
	if (setsockopt(srvs[idx].socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
	{
		std::cerr << "error: setsockopt() failed" << std::endl;
		close(srvs[idx].socket);
		exit(1);
	}

	if (bind(srvs[idx].socket, (struct sockaddr *)&srvs[idx].addr, sizeof(srvs[idx].addr)) < 0)
	{
		std::cerr << "error: bind call" << std::endl;
		close(srvs[idx].socket);
		exit(1);
	}

	if (listen(srvs[idx].socket, 20) < 0)
	{
		std::cerr << "error: listen call" << std::endl;
		close(srvs[idx].socket);
		exit(1);
	}
}

void WebServer::handle_accept(int i)
{
	socklen_t addrSize = sizeof(srvs[i].addr);
    clientSocket = accept(srvs[i].socket, (struct sockaddr *)&srvs[i].addr, &addrSize);
    if (clientSocket < 0)
    {
        std::cerr << "error: accept call" << std::endl;
        close(clientSocket);
        exit(1);
    }
    if (fcntl(clientSocket, F_SETFL, O_NONBLOCK, O_CLOEXEC) == -1)
    {
        std::cerr << "error: fcntl() failed" << std::endl;
        close(clientSocket);
        exit(1);
    }
}

int WebServer::find_server(int socket)
{
	for (size_t i = 0; i < servers.size(); i++)
	{
		if (srvs[i].socket == socket)
			return (i);
	}
	return (-1);
}

void WebServer::handle_receive(int i)
{
	int bytesReceived;
	char buf[1024];
	bzero(buf, 1024);

	while ((bytesReceived = recv(i, buf, sizeof(buf), 0)) > 0)
	{
		buffer.append(buf);
		if (bytesReceived < 1023)
			break ;
	}
	if (bytesReceived <= 0)
	{
		std::cerr << "error: recv() failed" << std::endl;
		close(i);
		exit(1);
	}
    else
    {
		Response res(buffer);
		buffer.clear();
	    std::string response = res.getResponse();
        int bytesSent = send(i, response.c_str(), response.size(), 0);
        if (bytesSent < 0)
        {
            std::cerr << "error: send call" << std::endl;
            close(i);
            exit (1);
        }
    }
}

void WebServer::run()
{
	fd_set read_fds;
	
	while (true)
	{
		read_fds = master;
		if (select(FD_SETSIZE, &read_fds, NULL, NULL, NULL) == -1)
		{
			std::cerr << "error: select call" << std::endl;
			exit(1);
		}
		for (int i = 0; i < FD_SETSIZE; i++)
		{
			if (FD_ISSET(i, &read_fds))
			{
				int var = find_server(i);
				if (var != -1)
					{
						handle_accept(var);
						FD_SET(clientSocket, &master);
					}
				else
					{
						handle_receive(i);
						FD_CLR(i, &master);
						close(i);
					}

			}
		}
	}
}
