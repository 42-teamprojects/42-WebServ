/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htalhaou <htalhaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 15:08:15 by htalhaou          #+#    #+#             */
/*   Updated: 2023/12/21 16:52:05 by htalhaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include "Cgi.hpp"
#include "Client.hpp"
# define BUFFER_SIZE 1024

class Request;

typedef struct s_client
{
    int socket;
	std::string buffer;
	size_t total_read;
	std::string response;
	size_t total_send;
	std::string method;
	bool chunked;
	size_t startCunter;
	size_t content_length;
} t_client;

typedef struct s_client_resp
{
    int socket;
	std::string response;
	size_t total_send;
} t_client_resp;

typedef struct s_server
{
	int socket;
	struct sockaddr_in addr;
} t_server;

class WebServer
{
private:
    std::vector<Server> servers;
    std::vector<t_server> srvs;
    int clientSocket;
    std::string buffer;
    fd_set master;
    fd_set read_fds;
	fd_set write_fds;
    fd_set tmpwrite_fds;
    std::vector<t_client> clients;
    std::vector<t_client_resp> clients_resp;
public:
    WebServer(std::vector<Server> &);
    ~WebServer();
    void handle_select(int port, int idx);
    void handle_accept(int i);
    std::string handle_receive(int i);
    void send_response(t_client_resp &client);
    void reset_client(t_client &client);
    void reset_client_resp(t_client_resp &client);
    int  find_socket(int socket);
    void run();
};
