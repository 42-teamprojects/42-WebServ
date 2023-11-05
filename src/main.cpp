/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htalhaou <htalhaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 22:27:57 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/05 20:22:14 by htalhaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int handle_select(int port)
{
    int serverSocket;
    struct sockaddr_in serverAddr;
    // socklen_t addrSize = sizeof(struct sockaddr_in);
    // fd_set master;
    // fd_set read_fds;

    // Create a socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // AF_INET: IPv4, SOCK_STREAM: TCP, 0: IP
    if (serverSocket < 0)
    {
        std::cerr << "error: socket creation" << std::endl;
	    exit(1);
    }

    // Set up server address information
    memset(&serverAddr, 0, sizeof(serverAddr)); // Zero out structure (sockaddr_in) in serverAddr (to avoid garbage) 
    serverAddr.sin_family = AF_INET; // IPv4
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // htonl converts a long integer (e.g. address) to a network representation 
    serverAddr.sin_port = htons(port); // htons converts a short integer (e.g. port) to a network representation
    int yes = 1;
    // lose the pesky "Address already in use" error message
    // setsockopt is used to allow the local address to be reused when the server is restarted before the required wait time expires
    // it works by setting the SO_REUSEADDR socket option
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
    {
        std::cerr << "error: setsockopt() failed" << std::endl;
		close(serverSocket);
		exit(1);
    }

    // Bind the socket to the server address
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "error: socket creation" << std::endl;
	    exit(1);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) < 0)
    {
        std::cerr << "error: listern call" << std::endl;
        exit(1);
    }
    return serverSocket;
}
int main()
{
    // int serverSocket, clientSocket;
    struct sockaddr_in clientAddr;
    socklen_t addrSize = sizeof(struct sockaddr_in);
    
    fd_set master;
    int clientSocket;
    fd_set read_fds;

    std::cout << "Server is listening on port 8080..." << std::endl;
    int var = handle_select(8080);
    int var2 = handle_select(8090);
    FD_ZERO(&master);
    FD_SET(var, &master);
    FD_SET(var2, &master);
    int fdmax = 2;

    char    buffer[2048] = {0};
    while (true)
    {
        read_fds = master;
        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)
        {
            std::cerr << "error: select call" << std::endl;
            exit(1);
        }
        for (int i = 0; i <= fdmax; i++)
        {
            if (FD_ISSET(i, &read_fds))
            {
                    clientSocket = accept(i, (struct sockaddr *)&clientAddr, &addrSize);
                    if (clientSocket < 0)
                    {
                        std::cerr << "error: accept call" << std::endl;
                        exit(1);
                    }
                    bzero(buffer, 2048);
                    int bytesReceived = recv(clientSocket, buffer, 2047, 0);
                    if (bytesReceived < 0)
                    {
                        std::cerr << "error: recv call" << std::endl;
                        exit(1);
                    }
                    std::cout << buffer << std::endl;
                    std::string response = "HTTP/1.1 200 OK\r\nServerContext: Tawafan/0.0 (Alaqssa)\r\n\r\n<html><body><h1>Welcome</h1></body></html>";
                    int bytesSent = send(clientSocket, response.c_str(), response.size(), 0);
                    if (bytesSent < 0)
                    {
                        std::cerr << "error: send call" << std::endl;
                        exit(1); 
                    }
                    close(clientSocket); // close the connection
            }
        }

        // // Accept incoming connections
        // clientSocket = accept(var, (struct sockaddr *)&clientAddr, &addrSize);
        // if (clientSocket < 0)
        // {
	    //     std::cerr << "error: accept call" << std::endl;
        //     exit(1);
        // }

        // // Handle the client connection
        
        // // if (connect(clientSocket, (struct sockaddr *)&clientAddr, sizeof(clientAddr)) < 0)
        // // {
        // //     std::cerr << "error: connect call" << std::endl;
        // //     exit(1);
        // // }
        
        // // Getting the request
        // char    buffer[2048];
        // int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        // if (bytesReceived < 0)
        // {
        //     std::cerr << "error: recv call" << std::endl;
        //     exit(1);
        // }
        // buffer[bytesReceived] = '\0';

        // std::cout << buffer << std::endl;

        // // Sending the response
        // std::string response = "HTTP/1.1 200 OK\r\nServerContext: Tawafan/0.0 (Alaqssa)\r\n\r\n<html><body><h1>Welcome</h1></body></html>";
        // int bytesSent = send(clientSocket, response.c_str(), response.size(), 0);
        // if (bytesSent < 0)
        // {
        //     std::cerr << "error: send call" << std::endl;
        //     exit(1); 
        // }

        // // Close the sockets when done
        // close(clientSocket);
    }
    close(var);

    return 0;
}
