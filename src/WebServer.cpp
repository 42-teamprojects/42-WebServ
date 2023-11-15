/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 15:08:25 by htalhaou          #+#    #+#             */
/*   Updated: 2023/11/15 18:57:41 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

WebServer::WebServer(std::vector<Server> &servers) : servers(servers)
{
	serverSocket = new int[servers.size() + 1];
	std::vector<Server>::iterator it = servers.begin();
	int i = 0;
	for (; it != servers.end(); ++it)
	{
		std::cout << "WebServer is listening on port " << it->getPort() << std::endl;
		handle_select(it->getPort(), i++);
	}
}

WebServer::~WebServer()
{
	for (size_t i = 0; i < servers.size(); i++)
		close(serverSocket[i]);
}

void WebServer::handle_select(int port, int idx)
{
	serverSocket[idx] = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket[idx] < 0)
	{
		std::cerr << "error: socket creation" << std::endl;
		exit(1);
	}

	if (fcntl(serverSocket[idx], F_SETFL, O_NONBLOCK, O_CLOEXEC) == -1)
	{
		std::cerr << "error: fcntl() failed" << std::endl;
		close(serverSocket[idx]);
		exit(1);
	}

	memset(&serverAddr[idx], 0, sizeof(serverAddr[idx]));
	serverAddr[idx].sin_family = AF_INET;
	serverAddr[idx].sin_port = htons(port);
	serverAddr[idx].sin_addr.s_addr = htonl(INADDR_ANY);

	int yes = 1;
	if (setsockopt(serverSocket[idx], SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
	{
		std::cerr << "error: setsockopt() failed" << std::endl;
		close(serverSocket[idx]);
		exit(1);
	}

	if (bind(serverSocket[idx], (struct sockaddr *)&serverAddr[idx], sizeof(serverAddr[idx])) < 0)
	{
		std::cerr << "error: bind call" << std::endl;
		close(serverSocket[idx]);
		exit(1);
	}

	if (listen(serverSocket[idx], 20) < 0)
	{
		std::cerr << "error: listen call" << std::endl;
		close(serverSocket[idx]);
		exit(1);
	}
}

void WebServer::handle_accept(int i)
{
	socklen_t addrSize = sizeof(serverAddr[i]);
    clientSocket = accept(i, (struct sockaddr *)&serverAddr[i], &addrSize);
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

void WebServer::handle_receive(int i)
{
	
	bzero(buffer, sizeof(buffer));
    int bytesReceived = recv(i, buffer, sizeof(buffer), 0);
    if (bytesReceived <= 0)
    {
        std::cerr << "error: recv() failed" << std::endl;
        close(i);
        exit (1);
    }
    else
    {
        std::cout << buffer << std::endl;
	    std::string response = "HTTP/1.1 200 OK\r\nServerContext: Tawafan/0.0 (Alaqssa)\r\n\r\n<html><body><h1>Welcome</h1></body></html>";
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
	fd_set master;
	fd_set read_fds;
	
	FD_ZERO(&master);
	FD_SET(serverSocket[0], &master);
    FD_SET(serverSocket[1], &master);
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
				if (serverSocket[0] == i || serverSocket[1] == i)
					{
						handle_accept(i);
						FD_SET(clientSocket, &master);
					}
				else
					{
						handle_receive(i);
						close(i);
						FD_CLR(i, &master);
					}

			}
		}
	}
}
