/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 21:37:17 by matef             #+#    #+#             */
/*   Updated: 2023/04/19 06:46:52 by matef            ###   ########.fr       */
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

struct HostAndPort
{
    string host;
    short port;
};

class SocketClass
{
    public:
        SocketClass(string config_file = "./config/webserv.conf");
        ~SocketClass();
        int create();
        bool bindSocket(int sockfd, SocketServer &server, HostAndPort hostAndPort);
        bool listenSocket(int sockfd);
        int sendFileInPackets(struct pollfd &fds);
        int communicate(struct pollfd &fds);
        void run();
        bool isNewConnection(int listener);


        struct  pollfd createPollfd(int sockfd);
        void    setFds();

        bool    isHeaderReceived(string request);

        bool recvError(int size, int fd);

        unsigned long hexToDec(string hex);

        int handlePostRequest(Client &client);
        int handleDeleteRequest(Client &client);
        void sendErrorReply(int i);

        string parseChunked(string body, int *c);
        Server getServer(int sockfd);
        Server getServer2(string host);
        Request                 httpRequest;
        
        void    closeConnection(int i);
        void    initResponse(int fd);

        bool isPortBelongToServer(Server server, short port);
        bool isHostBelongToServer(Server server, string host);
        bool isSeverNameBelongToServer(Server server, string serverName);

        bool isAllPortDiffrents(Server s1, Server s2);
    private:
        vector<Server>          servers;
        vector<SocketServer>    _s;
        vector<struct pollfd>   _fds;

        map<int, Client>        _clients;
        MimeTypes _mime;

};



#endif