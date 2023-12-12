/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htalhaou <htalhaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 15:08:25 by htalhaou          #+#    #+#             */
/*   Updated: 2023/12/12 20:22:42 by htalhaou         ###   ########.fr       */
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


Client& find_client(int socket, std::vector<Client> &clients)
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		// std::cout << clients[i].getSocket() << std::endl;
		if (clients[i].getSocket() == socket)
			return (clients[i]);
	}
	throw std::exception();
}

bool number_of(Client client, std::string c)
{
	(void)c;
	std::string buffer = client.getBuffer();
	std::cout << "buffer: " << buffer << std::endl;
	size_t pos = buffer.find("Content-Length:");
	if (pos != std::string::npos)
	{
		std::string tmp = buffer.substr(pos + 16);
		size_t pos2 = tmp.find("\r\n");
		std::cout << "tmp: " << tmp << std::endl;
		// std::cout << "====> pos2: " << pos2 << std::endl;
		if (pos2 != std::string::npos)
		{
			std::string tmp2 = tmp.substr(0, pos2);
			int content_length = atoi(tmp2.c_str());
			if (content_length == (int)((client.getTotalRead())))
				return (true);
		}
	}
	return (false);
}

void WebServer::handle_receive(int i, std::vector<Client> &clients)
{
	Client& client = find_client(i, clients);
    char buf[1025];
    int bytesReceived = recv(i, buf, 1024,0);
	buf[bytesReceived] = '\0';
    client.add_to_total_read(bytesReceived);
    std::string tmp = client.getBuffer();
    tmp.append(buf, bytesReceived);
    client.setBuffer(tmp);
				// std::cout << tmp;

		
	// }
    // if (bytesReceived <= 0)
	// {
    //     close(i);
    //     FD_CLR(i, &master);
    //     return;
    // }

	std::string buffer = client.getBuffer();
	// std::cout << "buffer: " << buffer << std::endl;
	size_t pos = buffer.find("Content-Length:");
	if (pos != std::string::npos)
	{
		std::string tmp = buffer.substr(pos + 16);
        size_t end = tmp.find_first_not_of("0123456789");
        std::string length = tmp.substr(0, end);
		// std::cout << "length: " << length << std::endl;
		int content_length = atoi(length.c_str());
		size_t pos2 = client.getBuffer().find("\r\n\r\n",pos);	
		if(pos2 != std::string::npos)
		{
			if((client.getTotalRead() - pos2 - 4) == static_cast<size_t>(content_length))
			{
				std::cout << "hello" << std::endl;
				// std::cout << client.getBuffer();
				// close(i);
				// FD_CLR(i, &master);
				// return;
				
			}
			// std::cout << client.getTotalRead() - pos2 - 4 << std::endl;
		}
	}

	// std::cout << "total_read: " << client.getTotalRead() << std::endl;
    // if (number_of(client, "Content-Length:") == true)
	// {
	// 	std::cout << "-=-= hnnnaa =-=-" << std::endl;
    //     std::cout << "buffer: " << client.getBuffer() << std::endl;
    //     Response res(client.getBuffer());
    //     client.getBuffer().clear();
    //     std::string response = res.getResponse();

    //     int bytesSent = 0;
    //     int totalBytesSent = 0;
    //     int responseSize = response.size();

    //     while (totalBytesSent < responseSize)
	// 	{
    //         bytesSent = send(i, response.c_str() + totalBytesSent, responseSize - totalBytesSent, 0);
    //         if (bytesSent == -1) {
    //             Console::error("Send() failed");
    //             close(i);
    //             FD_CLR(i, &master);
    //             return;
    //         }
    //         totalBytesSent += bytesSent;
    //         // response.erase(0, bytesSent);
    //     }
    //     close(i);
    //     FD_CLR(i, &master);
    // }
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
					// FD_CLR(i, &master);
					// close(i);
				}
			}
		}
	}
}
