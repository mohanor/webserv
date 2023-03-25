/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 03:41:14 by matef             #+#    #+#             */
/*   Updated: 2023/03/24 17:24:22 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
# include <fstream>

using namespace std;

#define ERROR -1


#define FILE_NOT_SET 0
#define READY_TO_SEND 1
#define SENDING 2
#define SENDED 3

#define PACKET_SIZE 1024

#define TIMEOUT 20000

class Client
{
    public:
        Client();
        Client(int fd);
        Client(string fileContent);
        Client(const Client &copy);
        Client &operator=(const Client &copy);
        ~Client();

        void setFileContent(string fileContent);
        string getFileContent();
        string getPacket();
        int getStatus();
        void setStatus(int status);
        double getTimeout();
        void setFd(int fd);
        void setFile(string file);

    private:
        int _fd;
        int _status;
        double _timeout;
        string _fileContent;
        ifstream* _file;
        char _packet[PACKET_SIZE];

};


#endif