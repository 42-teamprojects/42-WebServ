/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htalhaou <htalhaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 15:08:25 by htalhaou          #+#    #+#             */
/*   Updated: 2023/12/23 16:12:00 by htalhaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"
#include "webserv.hpp"
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


t_client_resp find_client(int socket, std::vector<t_client_resp> clients)
{
	int indx_client = -1;
	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (clients[i].socket == socket)
			indx_client = i;
	}
	if (indx_client == -1)
	{
		t_client_resp newClientResp;
		newClientResp.response = "";
		newClientResp.total_send = 0;
		newClientResp.socket = socket;
		clients.push_back(newClientResp);
		indx_client = clients.size() - 1;
	}
	return (clients[indx_client]);
}

t_client find_client(int socket, std::vector<t_client> clients)
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i].socket == socket)
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

std::string WebServer::handle_receive(int i)
{
	int client_index = -1;
	for(size_t indx = 0; indx < clients.size(); indx++)
	{
		if (clients[indx].socket == i)
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
		return "";
	}
	buf[bytesReceived] = '\0';
	std::string tmp(buf, bytesReceived);
	if(client_index == -1)
	{
		// size_t start = 0;
		t_client newClient;
		reset_client(newClient);
		size_t pos = tmp.find("Content-Length:");
		newClient.socket = i;
		size_t end = 0;
		if (pos != std::string::npos)
		{
			std::string Content = tmp.substr(pos + 16);
    	    end = Content.find_first_not_of("0123456789");
    	    std::string length = Content.substr(0, end);
			int content_length = atoi(length.c_str());
			newClient.content_length = content_length;
		}
		size_t pos2 = tmp.find("\r\n\r\n",end);
		if(pos2 != std::string::npos)
		{
			// start = pos2;
			newClient.startCunter = pos2;
		}
		size_t pos3 = tmp.find("Transfer-Encoding: chunked");
		if(pos3 != std::string::npos)
			newClient.chunked = true;
		else
			newClient.chunked = false;
		size_t pos4 = tmp.find(" ");
		if (pos4 != std::string::npos)
			newClient.method = tmp.substr(0, pos4);
		client_index = clients.size();
		clients.push_back(newClient);
	}
	if (bytesReceived <= 0)
	{
		close(i);
		FD_CLR(i, &master);
		return "";
	}
	clients[client_index].total_read += tmp.length();
	clients[client_index].buffer += tmp;

	if(clients[client_index].method == "POST")
	{
		if(clients[client_index].chunked)
		{
			size_t searchLength = std::min(static_cast<size_t>(10), static_cast<size_t>(clients[client_index].total_read));
			std::string substring = clients[client_index].buffer.substr(clients[client_index].total_read - searchLength, searchLength);
			size_t foundPos = substring.rfind("\r\n0\r\n");
			if (foundPos != std::string::npos)
			{
				std::string Request = clients[client_index].buffer;
				FD_CLR(clients[client_index].socket, &master);
				FD_SET(clients[client_index].socket, &write_fds);
				clients.erase(clients.begin() + client_index);
				return Request;
			}
		}
		else
		{
			if (clients[client_index].total_read - clients[client_index].startCunter >= clients[client_index].content_length)
			{
				std::string Request = clients[client_index].buffer;
				FD_CLR(clients[client_index].socket, &master);
				FD_SET(clients[client_index].socket, &write_fds);
				clients.erase(clients.begin() + client_index);
				return Request;

			}
		}
	}
	else
	{
		if (clients[client_index].buffer.find("\r\n\r\n") != std::string::npos)
		{
			std::string Request = clients[client_index].buffer;
			FD_CLR(clients[client_index].socket, &master);
			FD_SET(clients[client_index].socket, &write_fds);
			clients.erase(clients.begin() + client_index);
			return Request;
		}
	}
	return "";
	
}

void WebServer::send_response(t_client_resp &client, fd_set &master)
{
	int bytesSent = send(client.socket, client.response.c_str() + client.total_send, client.response.size() - client.total_send, 0);
	if (bytesSent < 0)
	{
		close(client.socket);
		return ;
	}
	client.total_send += bytesSent;
	if (client.total_send >= client.response.size())
	{
		FD_SET(client.socket, &master);
		FD_CLR(client.socket, &write_fds);
		reset_client_resp(client);
		close(client.socket);
		return ;
	}
}


void WebServer::reset_client(t_client &client)
{
	client.buffer = "";
	client.total_read = 0;
	client.method = "";
	client.chunked = false;
	client.startCunter = 0;
	client.content_length = 0;
	client.socket = -1;
}

void WebServer::reset_client_resp(t_client_resp &client)
{
	client.response = "";
	client.total_send = 0;
	client.socket = -1;
}



void WebServer::run()
{
	std::string request = "";
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
					request = handle_receive(i);
			}
			if (FD_ISSET(i, &tmpwrite_fds))
			{
				t_client_resp client = find_client(i, clients_resp);
				Response resp(request);
				client.response = resp.getResponse();
				send_response(client, master);
			}
		}
	}
}
