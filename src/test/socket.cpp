/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 21:39:23 by matef             #+#    #+#             */
/*   Updated: 2023/03/07 15:52:24 by matef            ###   ########.fr       */
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
    int listener;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0)
    {
        std::cerr << "socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    return listener;
}

void SocketClass::bindSocket(int listener)
{
    
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);


    int opt = 1;

    opt = setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (opt < 0)
    {
        std::cerr << "Error setting socket options" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    int bind_result = bind(listener, (struct sockaddr*) &address, sizeof(address));
    if (bind_result < 0)
    {
        std::cerr << "Error binding socket" << std::endl;
        exit(EXIT_FAILURE);
    }

}

void SocketClass::listenSocket(int listener)
{
    if (listen(listener, SOMAXCONN) < 0)
    {
        std::cerr << "Failed to listen for incoming connections" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void SocketClass::acceptSocket(int sockfd)
{
    
    
}

int SocketClass::sendFileInPackets(std::string file, struct pollfd *fds, int i)
{
    std::stringstream response;
    
    response << "HTTP/1.1 200 Ok\r\n";
    response << "Server: nginx/1.21.5\r\n";
    response << "Content-Type: text/html\r\n";
    response << "charset=utf-8\r\n";
    response << "Content-Length: " + std::to_string(getFileSize(file)) + "\r\n\r\n";

    if (getFileContent(file).size() > 1024)
    {
        send(fds[i].fd, response.str().c_str(), response.str().size(), 0);

        char buffer[1023] = { 0 };

        ifstream responsedFile(file.c_str(), ios::binary);

        while (responsedFile.read(buffer, 1024))
        {
            send(fds[i].fd, buffer, 1024, 0);
            std::cout << "buffer: " << buffer << std::endl;
            memset(buffer, 0, 1024);
        }

        int bytesRemaining = responsedFile.gcount();
        if (bytesRemaining > 0) {
            responsedFile.read(buffer, bytesRemaining);
            send(fds[i].fd, buffer, bytesRemaining, 0);
        }

        return 0;
    }

    response << getFileContent(file);
    send(fds[i].fd, response.str().c_str(), response.str().size(), 0);

    return 0;
}

int SocketClass::communicate(struct pollfd *fds, int i)
{
    int size;
    char buffer[1024] = { 0 };

    cout << "Descriptor " << fds[i].fd << " is readable" << endl;

    size = recv(fds[i].fd, buffer, sizeof(buffer), 0);
    if ( size < 0 )
    {
        cout << "recv failed" << endl; close(fds[i].fd);
        return false;
    }

    if ( !size )
    {
        cout << "Connection closed" << endl;
        close(fds[i].fd);
        return false;
    }

    cout << size << " bytes received" << endl;
    
    std::string file = "./www/html/index.html";

    sendFileInPackets(file, fds, i);

    return true;
}

void SocketClass::run()
{
    char buffer[1024] = { 0 };
    int counter = 0;
    int nfds = 1, current_size = 0;
    int addrlen;
    
    int listener = this->create();
    this->bindSocket(listener);
    this->listenSocket(listener);
    
    struct pollfd fds[20];
    memset(fds, 0, sizeof(fds));
    for(int i = 0; i < 20; i++)
        fds[i].fd = -1;

    fds[0].fd = listener;
    fds[0].events = POLLIN;

    int rc;
    int new_client;
    
    while (true)
    {

        cout << endl << "listen ..." << endl;
    
        rc = poll(fds, nfds, -1);
        if (rc <= 0) { cerr << "poll return " << rc << endl; break; }

        current_size = nfds;
        for(int i = 0; i < current_size; i++)
        {
            if (fds[i].revents == 0)
                continue;
            
            if (fds[i].revents != POLLIN)
            {
                cerr << "Error! revents = " << fds[i].revents << endl;
                close(fds[i].fd);
                fds[i].fd = -1;
                break;
            }

            if (fds[i].fd == listener)
            {
                
                new_client = accept(listener, (struct sockaddr*)&address , (socklen_t*)&addrlen);
                if (new_client < 0) { cerr << "fail to accept connection" << '\n'; return ; }

                fds[nfds].fd = new_client;
                fds[nfds].events = POLLIN;
                nfds++;

                cout << "New incoming connection " << new_client << endl;

            }
            else if ( !communicate(fds, i) )
                break;

        }

    }

}
