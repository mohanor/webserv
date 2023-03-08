/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 21:37:17 by matef             #+#    #+#             */
/*   Updated: 2023/03/07 15:40:22 by matef            ###   ########.fr       */
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

#define PORT 8080
const int MAX_REQUEST_SIZE = 1024;


class SocketClass
{
    public:
        SocketClass();
        ~SocketClass();
        int config(int ac, char**av);
        int create();
        void bindSocket(int sockfd);
        void listenSocket(int sockfd);
        void acceptSocket(int sockfd);
        int sendFileInPackets(std::string file, struct pollfd *fds, int i);
        int communicate(struct pollfd *fds, int i);
        void run();

    private:
        int port;
        struct sockaddr_in address;
};



#endif