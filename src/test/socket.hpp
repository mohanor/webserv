/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 21:37:17 by matef             #+#    #+#             */
/*   Updated: 2023/04/12 17:27:27 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

# ifndef SOCKET_HPP
# define SOCKET_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <math.h>

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

#include "../methods/get.hpp"
#include "../server/Worker.hpp"

struct SocketServer
{
    SocketServer(int sockfd, Server s) : sockfd(sockfd), server(s) {}
    int                 sockfd;
    Server              server;
    int                 addrlen;
    struct sockaddr_in address;
};

class SocketClass
{
    public:
        SocketClass();
        ~SocketClass();
        int create();
        bool bindSocket(int sockfd, SocketServer &server, short port);
        bool listenSocket(int sockfd);
        int sendFileInPackets(struct pollfd &fds);
        int communicate(struct pollfd &fds);
        void run();
        bool isNewConnection(int listener);


        struct  pollfd createPollfd(int sockfd);
        void    setFds();

        bool    isHeaderReceived(string request);

        bool recvError(int size, int fd);

        unsigned long hex2dec(string hex);

        bool handlePostRequest(Client &client);
        // bool handleChunkedRequest(Client &client);

        void uploadFile(Request request);
        
        string parseChunked(string body);
        Server getServer(int sockfd);
        Server getServer2(string host);
        Request                 httpRequest;
        
        void    closeConnection(int fd, int i);
        void    initResponse(int fd);
        void    createNewClient(int i);

        bool isPortBelongToServer(Server server, short port);
        bool isHostBelongToServer(Server server, string host);
        bool isSeverNameBelongToServer(Server server, string serverName);
    private:
        vector<Server>          servers;
        vector<SocketServer>    _s;
        vector<struct pollfd>   _fds;

        map<int, Client>        _clients;


        map<int, int>           _serverHandler;
    public:
        static MimeTypes               _mime;
};



#endif