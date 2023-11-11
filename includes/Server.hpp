/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htalhaou <htalhaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 15:08:15 by htalhaou          #+#    #+#             */
/*   Updated: 2023/11/11 17:25:48 by htalhaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.hpp"

class Server{
    private:
        struct sockaddr_in serverAddr[2];
        int serverSocket[2];
        int clientSocket;
        char buffer[1024];
    public:
        Server();
        ~Server();
        void handle_select(int port, int idx);
        void handle_accept(int i);
        void handle_receive(int i);
        void run();
};

#endif