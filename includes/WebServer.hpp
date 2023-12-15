/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htalhaou <htalhaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 15:08:15 by htalhaou          #+#    #+#             */
/*   Updated: 2023/12/15 16:36:12 by htalhaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include "Cgi.hpp"
#include "Client.hpp"


class Request;


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
public:
    WebServer(std::vector<Server> &);
    ~WebServer();
    void handle_select(int port, int idx);
    void handle_accept(int i, std::vector<Client> &clients);
    void handle_receive(int i, std::vector<Client> &clients);
    void handle_send(int i, std::vector<Client> &clients);
    void send_response(Client &client);
    int  find_socket(int socket);
    void run();
};
