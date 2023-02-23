#include <iostream>
#include "../main/webserv.hpp"

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


#define PORT 80

using namespace std;

int main(int ac, char **av)
{

    std::string config_file = "./config/webserv.conf";

    if (ac == 2)
        config_file = av[1];
    else if (ac > 2)
    {
        std::cout << "Usage: ./webserv <config_file>" << std::endl;
        return 1;
    }

    std::cout << "Config file: --> " << config_file << " <--" << std::endl;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        return 1;
    }

    int bind_status = bind(sockfd, NULL, 0);
    if (bind_status < 0)
    {
        perror("bind");
        return 1;
    }

    int listen_status = listen(sockfd, 0);
    if (listen_status < 0)
    {
        perror("listen");
        return 1;
    }

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    int client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd < 0)
    {
        perror("accept");
        return 1;
    }

    char buf[1024];
    int read_status = read(client_fd, buf, sizeof(buf));
    if (read_status < 0)
    {
        perror("read");
        return 1;
    }

    std::cout << "Read " << read_status << " bytes" << std::endl;
    std::cout << "Read: " << buf << std::endl;

    int write_status = write(client_fd, buf, read_status);
    if (write_status < 0)
    {
        perror("write");
        return 1;
    }

    std::cout << "Wrote " << write_status << " bytes" << std::endl;

    close(client_fd);
    close(sockfd);

    


    return 0;
}