/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 21:37:17 by matef             #+#    #+#             */
/*   Updated: 2023/04/02 01:41:21 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

# ifndef SOCKET_HPP
# define SOCKET_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <netinet/in.h>
#include <arpa/inet.h>

# include <poll.h>
# include <iostream>
# include <fstream>
#include <sstream>

#include <sys/stat.h>

#include "../utility/utility.hpp"
#include "../../parsing/request/Request.hpp"

#include "../../configuration/Configuration.hpp"
#include "../../parsing/mime/MimeTypes.hpp"

#include "Client.hpp"

#define ENDL "\r\n"
const int MAX_REQUEST_SIZE = 1024;
#define CHUNK_SIZE  1024
#define LOCALHOST "./www/html"


struct SocketServer
{
    SocketServer(int sockfd, int port, std::string host) : sockfd(sockfd), port(port), host(host) {}
    int                 sockfd;
    int                 port;
    std::string         host;
    int                 addrlen;
    struct sockaddr_in address;
};

class SocketClass
{
    public:
        SocketClass();
        ~SocketClass();
        int create();
        void bindSocket(int sockfd, SocketServer &server);
        void listenSocket(int sockfd);
        void acceptSocket(int sockfd);
        int sendFileInPackets(string file, struct pollfd &fds);
        int communicate(struct pollfd &fds);
        void run();
        bool isNewConnection(int listener);

        string  joinRootAndPath(string root, string path, Request &httpRequest);
        struct  pollfd createPollfd(int sockfd);
        void    setFds();

        bool    isHeaderReceived(string request);

        bool recvError(int size, int fd);

        void handlePostRequest(Client &client);
        // bool handleChunkedRequest(Client &client);

        void uploadFile(Client &client);
        
    private:
        struct sockaddr_in      address;
        vector<Server>          servers;
        vector<SocketServer>    _s;
        vector<struct pollfd>   _fds;

        map<int, Client>        _clients;

        Request                 httpRequest;
        MimeTypes               _mimeTypes;
};



#endif