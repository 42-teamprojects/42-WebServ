/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htalhaou <htalhaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 15:08:25 by htalhaou          #+#    #+#             */
/*   Updated: 2023/12/10 21:05:51 by htalhaou         ###   ########.fr       */
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

void WebServer::handle_accept(int i, std::vector<Client> &clients)
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
	Client client(clientSocket);
	clients.push_back(client);
	FD_SET(clientSocket, &master);
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

bool number_of(std::string str, std::string c)
{
	(void)c;
	// std::cout << str << std::endl;
	// exit(0);
	if (str.rfind("--\r\n") != std::string::npos)
		return (true);
	return (false);
}

Client& find_client(int socket, std::vector<Client> &clients)
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i].getSocket() == socket)
			return (clients[i]);
	}
	throw std::exception();
}

void WebServer::handle_receive(int i, std::vector<Client> &clients)
{
	Client& client = find_client(i, clients);
	int bytesReceived = 0;
	char buf[1024];
	if ((bytesReceived = recv(i, buf, 1024, 0)) <= 0)
	{
		close(i);
		FD_CLR(i, &master);
		return ;
	}
	std::string tmp;
	tmp.append(buf, bytesReceived);
	client.setBuffer(tmp);
	if(bytesReceived > 0)
    {
		// std::cout << "buffer: " << client.getBuffer() << std::endl;
		Response res(client.getBuffer());
		client.getBuffer().clear();
		std::string response = res.getResponse();
		int bytesSent = 0;
		int totalBytesSent = 0;
		int responseSize = response.size();
		while (totalBytesSent < responseSize)
		{
			bytesSent = send(i, response.c_str() + totalBytesSent, responseSize - totalBytesSent, 0);
			if (bytesSent == -1)
			{
				Console::error("Send() failed");
			}
			totalBytesSent += bytesSent;
			response.erase(0, bytesSent);
		}
		close(i);
		FD_CLR(i, &master);
		clients.erase(clients.begin());
    }
}

void WebServer::run()
{
	fd_set read_fds;
    std::vector<Client> clients;
	
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
					handle_accept(var, clients);
					FD_SET(clientSocket, &master);
				}
				else
				{
					handle_receive(i, clients);
					FD_CLR(i, &master);
					close(i);
				}
			}
		}
	}
}
