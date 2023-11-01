/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 22:27:57 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/01 21:36:50 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

int main() {
    // Holds the ip and port of the server
    struct sockaddr_in serv_addr, cli_addr;
    // int opt = 1;
    int new_socket;
    socklen_t len = sizeof(cli_addr);

    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    // Forcefully attaching socket to the port 8080
    // if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    //     perror("setsockopt");
    //     exit(EXIT_FAILURE);
    // }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(8080);
    
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 4) < 0) {
        perror("Listenning failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "Listenning on port 8080" << std::endl;
    if ((new_socket = accept(sockfd, (struct sockaddr *)&cli_addr, &len)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE); 
    }
    
    close(new_socket);
    close(sockfd);
    return 0;
}
