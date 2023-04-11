/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 03:41:14 by matef             #+#    #+#             */
/*   Updated: 2023/04/09 21:30:57 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <map>
#include "../../parsing/request/Request.hpp"

using namespace std;

#define ERROR -1

// #define EMPTY 0
// #define HEADER_RECEIVED 1
// #define FILE_NOT_SET 1
// #define READY_TO_SEND 2
// #define SENDING 3
// #define SENDED 4

enum Status
{
    EMPTY,
    REDING,
    HEADER_RECEIVED,
    FILE_NOT_SET,
    READY_TO_SEND,
    SENDING,
    SENDED
};

#define PACKET_SIZE 1024

#define TIMEOUT 20000

#define ENDL "\r\n"


class Client
{
    public:
        Client();
        Client(int fd);
        Client(const Client &copy);
        Client &operator=(const Client &copy);
        ~Client();

        void        setFileContent(string fileContent);
        string      getFileContent();
        string      getPacket();
        int         getStatus();
        void        setStatus(int status);
        double      getTimeout();
        void        setFd(int fd);
        void        setHeader(string header);
        string      getHeader();
        // string  getRequest();

        void        receiveHeader();

        void        setRequestHeader(map<string, string> header_map);
        void        setHeaderReceivedVar(bool val);
        
        bool        isHeaderRecived();
        void        setRequest(Request request);
        Request     getRequest();
        
        void        setIsUploading(bool isUploading);
        bool        isUploading();
        void        setBody();
        string      getBody();
        
        void        setContentLength(size_t size);
        size_t      getContentLength();

        void       setMimeType(string mimeType);
        string     getMimeType();

        size_t      getReceivedLength();
        string      _request_string;
        string      _requestString;
        Request     _request;
        size_t         _request_length;
        

    private:
        int                     _fd;
        int                     _status;
        double                  _timeout;
        string                  _fileContent;
        char                    _packet[PACKET_SIZE];
        string                  _header;
        string                  _body;
        bool                    _isUploading;
        bool                    _headerReceived;
        map<string, string>     _header_map;
        size_t                  _contentLength;
        string                  _mimeType;
};


#endif