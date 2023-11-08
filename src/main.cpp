/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htalhaou <htalhaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 22:27:57 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/08 20:02:15 by htalhaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int handle_select(int port) {
    int serverSocket;
    struct sockaddr_in serverAddr;
    
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "error: socket creation" << std::endl;
        exit(1);
    }

    // if (fcntl(serverSocket, F_SETFL, O_NONBLOCK | FD_CLOEXEC) == -1) {
    //     std::cerr << "error: fcntl() failed" << std::endl;
    //     close(serverSocket);
    //     exit(1);
    // }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port);
    int yes = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0) {
        std::cerr << "error: setsockopt() failed" << std::endl;
        close(serverSocket);
        exit(1);
    }
    
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "error: bind call" << std::endl;
        close(serverSocket);
        exit(1);
    }

    if (listen(serverSocket, 5) < 0) {
        std::cerr << "error: listen call" << std::endl;
        close(serverSocket);
        exit(1);
    }

    return serverSocket;
}

int main()
{
    fd_set master;
    int clientSocket;
    fd_set read_fds;

    std::cout << "Server is listening on ports 8080 and 8090..." << std::endl;
    int var = handle_select(8080);
    int var2 = handle_select(8090);
    FD_ZERO(&master);
    FD_SET(var, &master);
    FD_SET(var2, &master);
    int fdmax = std::max(var, var2);

    char buffer[2048] = {0};

    while (true)
    {
        read_fds = master;
        if (select(fdmax + 1, &read_fds, nullptr, nullptr, nullptr) == -1)
        {
            std::cerr << "error: select call" << std::endl;
            exit(1);
        }

        for (int i = 0; i <= fdmax; i++)
        {
            if (FD_ISSET(i, &read_fds))
            {
                if (i == var || i == var2)
                {
                    struct sockaddr_in clientAddr;
                    socklen_t addrSize = sizeof(struct sockaddr_in);
                    clientSocket = accept(i, (struct sockaddr *)&clientAddr, &addrSize);
                    if (clientSocket < 0)
                    {
                        std::cerr << "error: accept call" << std::endl;
                        close(clientSocket);
                        exit(1);
                    }

                    if (fcntl(clientSocket, F_SETFL, O_NONBLOCK | FD_CLOEXEC) == -1)
                    {
                        std::cerr << "error: fcntl() failed" << std::endl;
                        close(clientSocket);
                        exit(1);
                    }
                    FD_SET(clientSocket, &master);
                    fdmax = std::max(fdmax, clientSocket);
                }
                else
                {
                    int bytesReceived = recv(i, buffer, sizeof(buffer), 0);
                    if (bytesReceived < 0)
                    {
                        std::cerr << "error: recv() failed" << std::endl;
                        close(i);
                        FD_CLR(i, &master);
                        exit (1);
                    }
                    else if (bytesReceived == 0)
                    {
                        std::cout << "Client disconnected" << std::endl;
                        close(i);
                        FD_CLR(i, &master);
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
                            FD_CLR(i, &master);
                            close(i);
                            exit (1);
                        }
                    }
                    memset(buffer, 0, sizeof(buffer));
                }
            }
        }
    }
    close(var);
    close(var2);

    return 0;
}