/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htalhaou <htalhaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 15:08:15 by htalhaou          #+#    #+#             */
/*   Updated: 2023/12/11 22:51:51 by htalhaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include "Cgi.hpp"
#include "Client.hpp"

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
public:
    WebServer(std::vector<Server> &);
    ~WebServer();
    void handle_select(int port, int idx);
    void handle_accept(int i, std::vector<Client> &clients);
    void handle_receive(int i, std::vector<Client> &clients);
    int  find_socket(int socket);
    void run();
};
