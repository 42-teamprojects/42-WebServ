/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htalhaou <htalhaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 15:08:25 by htalhaou          #+#    #+#             */
/*   Updated: 2023/12/02 18:06:36 by htalhaou         ###   ########.fr       */
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
	std::string ports = "\033[1;32m";
	std::vector<int> portsList;
	for (; it != servers.end(); ++it)
	{
		if (std::find(portsList.begin(), portsList.end(), it->getPort()) != portsList.end()) {
			Console::warning("Port " + toString(it->getPort()) + " is duplicated");
			continue;
		}
		ports += toString(it->getPort()) + (it != servers.end() - 1 ? ", " : "");
		portsList.push_back(it->getPort());
		handle_select(it->getPort(), i++);
	}
	Console::info("WebServ is listening on ports " + ports + "\033[0m");
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
		Console::error("socket() failed");
		return ;
	}
	if (fcntl(srvs[idx].socket, F_SETFL, O_NONBLOCK, O_CLOEXEC) == -1)
	{
		Console::error("fcntl() failed");
		close(srvs[idx].socket);
		return ; 
	}
	FD_SET(srvs[idx].socket, &master);

	memset(&srvs[idx].addr, 0, sizeof(srvs[idx].addr));
	srvs[idx].addr.sin_family = AF_INET;
	srvs[idx].addr.sin_port = htons(port);
	srvs[idx].addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int yes = 1;
	if (setsockopt(srvs[idx].socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
	{
		Console::error("setsockopt() failed");
		close(srvs[idx].socket);
		return ; 
	}

	if (bind(srvs[idx].socket, (struct sockaddr *)&srvs[idx].addr, sizeof(srvs[idx].addr)) < 0)
	{
		Console::error("Bind() failed");
		close(srvs[idx].socket);
		return ;
	}

	if (listen(srvs[idx].socket, 20) < 0)
	{
		Console::error("Listen() failed");
		close(srvs[idx].socket);
		return ;
	}
}

void WebServer::handle_accept(int i)
{
	socklen_t addrSize = sizeof(srvs[i].addr);
    clientSocket = accept(srvs[i].socket, (struct sockaddr *)&srvs[i].addr, &addrSize);
    if (clientSocket < 0)
    {
		Console::error("Accept() failed");
        close(clientSocket);
        return ; 
    }
    if (fcntl(clientSocket, F_SETFL, O_NONBLOCK, O_CLOEXEC) == -1)
    {
		Console::error("fcntl() failed");
        close(clientSocket);
        return; 
    }
}

int WebServer::find_socket(int socket)
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
	char buf[99999];
	bzero(buf, 99999);

	while ((bytesReceived = recv(i, buf, sizeof(buf), 0)) > 0)
	{
		buffer.append(buf, bytesReceived);
		if (bytesReceived < 99999)
			break ;
	}
	if (bytesReceived <= 0)
	{
		close(i);
		Console::warning("Client " + toString(i) + " disconnected");
		return ; 
	}
    else
    {
		Response res(buffer);
		buffer.clear();
		std::string response = res.getResponse();
		int bytesSent = 0;
		int totalBytesSent = 0;
		int responseSize = response.size();
		while (totalBytesSent < responseSize)
		{
			bytesSent = send(i, response.c_str() + totalBytesSent, responseSize - totalBytesSent, 0);
			if (bytesSent < 0)
			{
				Console::error("Send() failed");
				send(i, response.c_str() + totalBytesSent, responseSize - totalBytesSent, 0);
				break ;
			}
			if (bytesSent == 0)
				break ;
			totalBytesSent += bytesSent;
			response.substr(totalBytesSent);
			if (bytesSent == 0)
				break ;
		}
		close(i);
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
			Console::error("Select() failed");
			return ; 
		}
		for (int i = 0; i < FD_SETSIZE; i++)
		{
			if (FD_ISSET(i, &read_fds))
			{
				int var = find_socket(i);
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
