/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 15:08:15 by htalhaou          #+#    #+#             */
/*   Updated: 2023/11/16 12:32:28 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

class WebServer
{
private:
    std::vector<Server> servers;
    struct sockaddr_in *serverAddr;
    int *serverSocket;
    int clientSocket;
    char buffer[999999];

public:
    WebServer(std::vector<Server> &);
    ~WebServer();
    void handle_select(int port, int idx);
    void handle_accept(int i);
    void handle_receive(int i);
    void run();
};
