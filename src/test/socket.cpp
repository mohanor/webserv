/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 21:39:23 by matef             #+#    #+#             */
/*   Updated: 2023/03/24 20:02:14 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"

SocketClass::SocketClass()
{
    servers = Configuration().getServers();
}

SocketClass::~SocketClass()
{
}

int SocketClass::create()
{
    int listener;

    listener = socket(AF_INET, SOCK_STREAM, 0); // AF_INET = IPv4, SOCK_STREAM = TCP, 0 = default protocol of socket type
    if (listener < 0)
    {
        cerr << "socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    return listener;
}

void SocketClass::bindSocket(int listener, SocketServer &server)
{
    bzero(&server.address, sizeof(server.address));
    
    server.address.sin_family = AF_INET;
    server.address.sin_addr.s_addr = INADDR_ANY;
    server.address.sin_port = htons(server.port);

    int opt = 1;

    opt = setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (opt < 0)
    {
        cerr << "Error setting socket options" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    int bind_result = ::bind(listener, (struct sockaddr*) &server.address, sizeof(server.address));
    if (bind_result < 0)
    {
        cerr << "Error binding socket" << std::endl;
        exit(EXIT_FAILURE);
    }

}

void SocketClass::listenSocket(int listener)
{
    if (listen(listener, SOMAXCONN) < 0)
    {
        cerr << "Failed to listen for incoming connections" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void SocketClass::acceptSocket(int sockfd)
{
}

int SocketClass::sendFileInPackets(std::string file, struct pollfd &fds)
{
    MimeTypes   mimeTypes;
    
    string mimeType = mimeTypes.getMimeType(mimeTypes.getExtension(file));

    if (_clients[fds.fd].getStatus() == FILE_NOT_SET)
    {
        _clients[fds.fd].setFileContent(getFileContent(file));
        _clients[fds.fd].setStatus(READY_TO_SEND);
        
    }

    if (_clients[fds.fd].getStatus() == READY_TO_SEND)
    {
        string      response;

        response = "HTTP/1.1 200 Ok" ENDL;
        response += "Content-Type: " + mimeType + ENDL;
        response += "Content-Length: " + to_string(getFileSize(file)) + ( ENDL ENDL );
        
        _clients[fds.fd].setStatus(SENDING);
        send(fds.fd, response.c_str(), response.size(), 0);

        return 0;
    }

    string packet = _clients[fds.fd].getPacket();
    send(fds.fd, packet.c_str(), packet.size(), 0);

    return 0;
}

string SocketClass::joinRootAndPath(string root, string path, Request &httpRequest)
{
    if (httpRequest.getRessource() == "/")
        return "./www/html/index.html";

    return "./www/html" + httpRequest.getRessource();
}

int SocketClass::communicate(struct pollfd &fds)
{
    int     size;
    char    request[CHUNK_SIZE];

    size = recv(fds.fd, request, sizeof(request), 0);
    
    if ( size < 0 )
    {
        cerr << "recv failed" << endl;
        close(fds.fd); return false;
    }
    if ( !size )
    {
        cerr << "Connection closed" << endl;
        close(fds.fd);
        return false;
    }


    httpRequest = Request(Request::deserialize(request));
    fds.events = POLLOUT;

    Client client(fds.fd);

    _clients.insert(make_pair(fds.fd, client));

    return true;
}


bool SocketClass::isNewConnection(int listener)
{
    for (size_t i = 0; i < _s.size(); i++)
    {
        if (_s[i].sockfd == listener)
            return true;
    }
    return false;
}

struct pollfd SocketClass::createPollfd(int sockfd)
{
    struct pollfd pfd;
    pfd.fd = sockfd;
    pfd.events = POLLIN;
    return pfd;
}

void SocketClass::setFds()
{
    int listener;

    for (vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
    {
        listener = this->create();
        
        _s.push_back(SocketServer(listener, it->getPort(), it->getHost()));

        this->bindSocket(listener, _s[_s.size() - 1]);
        this->listenSocket(listener);
        _fds.push_back(createPollfd(listener));

    }
}

void SocketClass::run()
{
    int     current_size;
    int     newClient;

    setFds();
    
    cout << "listen ..." << endl;
    while (true)
    {
        if (poll(&_fds[0], _fds.size(), -1) < 0) break;

        current_size = _fds.size();
        for(int i = 0; i < current_size; i++)
        {
            if (_fds[i].revents == 0)
                continue;
            
            if (_fds[i].revents & POLLIN)
            {
                if (isNewConnection(_fds[i].fd))
                {
                    newClient = accept(_s[i].sockfd, (struct sockaddr*)&_s[i].address, (socklen_t*)&_s[i].addrlen);
                    if (newClient < 0) { cerr << "fail to accept connection" << '\n'; return ; }

                    _fds.push_back(createPollfd(newClient));
                }
                else if ( !communicate(_fds[i]) )
                    break;
            }

            if (_fds[i].revents & POLLOUT)
            {
                httpRequest.resource();
                
                string file = joinRootAndPath(LOCALHOST, httpRequest.getRessource(), httpRequest);
                
                sendFileInPackets(file, _fds[i]);
                
                if (_clients[_fds[i].fd].getStatus() == SENDED)
                {
                    close(_fds[i].fd);
                    _clients.erase(_fds[i].fd);
                    _fds.erase(_fds.begin() + i);
                }
                
            }

        }

    }

}
