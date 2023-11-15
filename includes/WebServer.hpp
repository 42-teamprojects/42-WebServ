/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htalhaou <htalhaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 15:08:15 by htalhaou          #+#    #+#             */
/*   Updated: 2023/11/15 15:45:09 by htalhaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "webserv.hpp"

class WebServer{
    private:
        struct sockaddr_in serverAddr[2];
        int serverSocket[2];
        int clientSocket;
        char buffer[10000];
    public:
        WebServer();
        ~WebServer();
        void handle_select(int port, int idx);
        void handle_accept(int i);
        void handle_receive(int i);
        void run();
};
