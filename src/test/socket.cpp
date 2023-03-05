/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 21:39:23 by matef             #+#    #+#             */
/*   Updated: 2023/03/05 19:15:51 by yoelhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"

SocketClass::SocketClass() : port(PORT)
{
}

SocketClass::~SocketClass()
{
}

int SocketClass::config(int ac, char **av)
{
    std::string config_file = "./config/default.conf";

    if (ac == 2)
        config_file = av[1];
    else if (ac > 2)
    {
        std::cout << "Usage: ./webserv <config_file>" << std::endl;
        return 1;
    }

    std::cout << "Config file: --> " << config_file << " <--" << std::endl;
    std::cout << getFileContent(config_file);
    return 0;
}

int SocketClass::create()
{
    int sockfd;

    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
    {
        std::cerr << "socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    return sockfd;
}

void SocketClass::bindSocket(int sockfd)
{
    // associates a socket with a specific network address and port number on the local machine.
    /*
    
        struct sockaddr_in {
            short            sin_family;   // e.g. AF_INET
            unsigned short   sin_port;     // e.g. htons(3490)
            struct in_addr   sin_addr;     // see struct in_addr, below
            char             sin_zero[8];  // zero this if you want to
        };
    
    */
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    int bind_result = bind(sockfd, (struct sockaddr*) &address, sizeof(address));
    if (bind_result < 0)
    {
        std::cerr << "Error binding socket" << std::endl;
        exit(EXIT_FAILURE);
    }

}

void SocketClass::listenSocket(int sockfd)
{
    if (listen(sockfd, 5) == -1)
    {
        std::cerr << "Failed to listen for incoming connections" << std::endl;
        exit(1);
    }
}



void SocketClass::acceptSocket(int sockfd)
{
    int clientSocket;
    struct sockaddr_in clientAddr;
    char request[MAX_REQUEST_SIZE];
    socklen_t addrLen = sizeof(struct sockaddr_in);

    while (true)
    {
        clientSocket = accept(sockfd, (struct sockaddr*)&clientAddr, &addrLen);
        if (clientSocket == -1)
        {
            std::cerr << "Failed to accept incoming connection" << '\n';
            continue;
        }

        int numBytes = recv(clientSocket, request, MAX_REQUEST_SIZE, 0);    
        if (numBytes == -1)
        {
            std::cerr << "Failed to read request from client" << '\n';
            close(clientSocket);
            continue;
        }

        
        Request r(Request::deserialize(request));

        // r.
        

        const char* response = "this page successfully requested\n";
        
        if (send(clientSocket, response, strlen(response), 0) == -1)
            std::cerr << "Failed to send response to client" << '\n';

        close(clientSocket);
    }
}

void SocketClass::run()
{
    int socket_fd = this->create();
    
    this->bindSocket(socket_fd);
    this->listenSocket(socket_fd);
    this->acceptSocket(socket_fd);

}
