/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htalhaou <htalhaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 15:08:25 by htalhaou          #+#    #+#             */
/*   Updated: 2023/12/20 14:47:47 by htalhaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"
#include "webserv.hpp"
size_t byt=0;
size_t content_lenghtt=0;
std::string buuuufer = "";
size_t start = 0;
WebServer::WebServer(std::vector<Server> &servers) : servers(servers)
{
	srvs.resize(servers.size() + 1);	
	std::vector<Server>::iterator it = servers.begin();
	FD_ZERO(&master); 
	FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);
	FD_ZERO(&tmpwrite_fds);
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
	// Client client(clientSocket);
	// clients.push_back(client);
	// FD_SET(clientSocket, &master);
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
		if (clients[i].getSocket() == socket)
			return (clients[i]);
	}
	throw std::exception();
}
bool check_chunked(std::string header)
{
	if (header.find("Transfer-Encoding: chunked") != std::string::npos)
		return (true);
	return (false);
}

bool number_of(Client client)
{	
	if (client.getmethod() == "POST")
	{
		if(client.getchunked() == true)
		{
			size_t searchLength = std::min(static_cast<size_t>(10), static_cast<size_t>(client.getTotalRead()));
			std::string substring = client.getBuffer().substr(client.getTotalRead() - searchLength, searchLength);
			size_t foundPos = substring.rfind("\r\n0\r\n");
			if (foundPos != std::string::npos)
			{
				std::cout << "i get all request in chunked" << std::endl;
				return (true);
			}
		}
		else
		{
			if(client.getstartCunter() != 0)
			{
				size_t lll = (client.getTotalRead() - client.getstartCunter() - 4);
				if( lll >= client.getcontent_length())
				{
					return (true);
				}
			}
		}
	}
	else
	{
		if (client.getBuffer().find("\r\n\r\n") != std::string::npos)
		{
			return (true);
		}
	}
	return (false);
}

void WebServer::handle_receive(int i)
{
	int client_index = -1;
	for(size_t indx = 0; indx < clients.size(); indx++)
	{
		if (clients[indx].getSocket() == i)
		{
			client_index = indx;
			break;
		}
	}
	char buf[BUFFER_SIZE + 1];
	int bytesReceived = recv(i, buf, BUFFER_SIZE, 0);
	if(bytesReceived <= 0)
	{
		close(i);
		FD_CLR(i, &master);
		return;
	}
	buf[bytesReceived] = '\0';
	std::string tmp(buf, bytesReceived);
	if(client_index == -1)
	{
		Client newClient;

		size_t pos = tmp.find("Content-Length:");
		newClient.setSocket(i);
		size_t end = 0;
		if (pos != std::string::npos)
		{
			std::string Content = tmp.substr(pos + 16);
    	    end = Content.find_first_not_of("0123456789");
    	    std::string length = Content.substr(0, end);
			int content_length = atoi(length.c_str());
			content_lenghtt = content_length;
			newClient.setcontent_length(content_length);
		}
		size_t pos2 = tmp.find("\r\n\r\n",end);
		if(pos2 != std::string::npos)
		{
			start = pos2;
			newClient.setstartCunter(pos2);
		}
		size_t pos3 = tmp.find("Transfer-Encoding: chunked");
		if(pos3 != std::string::npos)
			newClient.setchunked(true);
		else
			newClient.setchunked(false);
		size_t pos4 = tmp.find(" ");
		if (pos4 != std::string::npos)
			newClient.setmethod(tmp.substr(0, pos4));
		client_index = clients.size();
		clients.push_back(newClient);
	}
	if (bytesReceived <= 0)
	{
		close(i);
		FD_CLR(i, &master);
		return;
	}
	byt += tmp.length();
	buuuufer.append(tmp);
	std::cout << byt - start << " " << content_lenghtt << std::endl;
	if(byt - start >= content_lenghtt)
	{
		std::cout << "hi" << std::endl;
		FD_CLR(clients[client_index].getSocket(), &master);
		close(clients[client_index].getSocket());
		// if(clients[client_index].getmethod() == "POST")
		// {
		// 	// exit(0);
		// }
		// std::cout << buuuufer << std::endl;
	}
	// clients[client_index].add_to_total_read(tmp.length());
	// clients[client_index].setappendbuffer(tmp);
	// if (number_of(clients[client_index]) == true)
	// {
	// 	FD_CLR(clients[client_index].getSocket(), &master);
	// 	FD_SET(clients[client_index].getSocket(), &write_fds);
	// 	clients.erase(clients.begin() + client_index);
	// 	// return ;
	// }
}

void WebServer::send_response(Client &client)
{
	int sended = send(client.getSocket(), client.getResponse().c_str() + client.getTotalSend(), client.getResponse().size() - client.getTotalSend(), 0);
	if (sended <= 0)
	{
		Console::error(std::strerror(errno));
		close(client.getSocket());
		return ;
	}
	client.add_to_total_send(sended);
	if (client.getTotalSend() >= client.getResponse().size())
	{
		FD_CLR(client.getSocket(), &write_fds);
		// FD_SET(client.getSocket(), &master);
		close(client.getSocket());
		// client.reset();
		
		
		// return ;
	}
	
}

void WebServer::run()
{
	// struct timeval timeout;
	// timeout.tv_sec = 5;
	// timeout.tv_usec = 0;

	while (true)
	{
		read_fds = master;
		tmpwrite_fds = write_fds;
		if (select(FD_SETSIZE, &read_fds, &tmpwrite_fds, NULL, NULL) < 0)
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
				}
			}
			if (FD_ISSET(i, &tmpwrite_fds))
			{
				Client &client = find_client(i, clients);
				// std::cout << client.getBuffer() << std::endl;
				Response response(client.getBuffer());
				client.setResponse(response.getResponse());
				send_response(client);
				
			}
		}
	}
}
