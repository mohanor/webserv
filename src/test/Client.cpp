/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 03:41:11 by matef             #+#    #+#             */
/*   Updated: 2023/04/15 20:48:34 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"


Client::Client()
{
    _status = REDING;
    _headerReceived = false;
    _isUploading = false;
    _request_length = 0;
}

Client::Client(int fd)
{
    (void)fd;
    _status = REDING;
    _headerReceived = false;
    _isUploading = false;
    _request_length = 0;
}

void Client::setHeaderReceivedVar(bool val)
{
    _headerReceived = val;
}

bool Client::isHeaderRecived()
{
    return _headerReceived;
}


Client::Client(const Client &copy)
{
    *this = copy;
}

Client &Client::operator=(const Client &copy)
{
    if (this != &copy)
    {
        _status = copy._status;
        _timeout = copy._timeout;
        _fileContent = copy._fileContent;
    }
    return *this;
}

Client::~Client() {}

void Client::setFileContent(string fileContent)
{
    _fileContent = fileContent;
}

// void    Client::setFile(const string &file)
// {
//     _file = new ifstream(file);
// }

string Client::getFileContent()
{
    return _fileContent;
}

void Client::setHeader(string header)
{
    _header += header;
}

// void Client::setRequest(string request)
// {
//     _request += request;
// }

// string Client::getRequest()
// {
//     return _request;
// }

string Client::getHeader()
{
    
    string header = _requestString.substr(0, _requestString.find("\r\n\r\n"));

    // _requestString.erase(0, index);
    return header;
}

size_t Client::getReceivedLength()
{
    return _requestString.size() - getHeader().size() - 4;
}

void Client::setComment(string comment)
{
    _respComment = comment;
}

string Client::getComment()
{
    return _respComment;
}

void Client::setRespStatus(string status)
{
    _respStatus = status;
}

string Client::getRespStatus()
{
    return _respStatus;
}

void Client::setReturn(string ret)
{
    _return = ret;
}

string Client::getReturn()
{
    return _return;
}

void Client::setCgiHeader(map<string, string> header)
{
    _cgiHeader = header;
}

map<string, string> Client::getCgiHeader()
{
    return _cgiHeader;
}

string Client::getPacket()
{
    if (_fileContent.size() == 0)
    {
        _status = SENDED;
        return "";
    }

    if (_fileContent.size() < PACKET_SIZE)
    {
        string packet = _fileContent;
        _fileContent = "";
        _status = SENDED;
        return packet;
    }
    
    string packet = _fileContent.substr(0, PACKET_SIZE);
    _fileContent.erase(0, PACKET_SIZE);

    return packet;
    /*
    if (_status == READY_TO_SEND)
        cout << "ready to send" << endl;
    int bytesRemaining = (*_file).gcount();
    cout << "bytes remaining: " << bytesRemaining << endl;
    if (bytesRemaining > PACKET_SIZE)
    {
        _status = SENDING;
        (*_file).read(_packet, PACKET_SIZE);
        return _packet;
    }
    
    if (bytesRemaining > 0)
    {
        char buffer[1023] = { 0 };
        _status = SENDED;
        (*_file).read(buffer, bytesRemaining);
        return _packet;
    }
    

    _status = SENDED;
    return "";
    */
}

int Client::getStatus()
{
    return _status;
}

void Client::setStatus(int status)
{
    _status = status;
}

string      Client::getBody()
{
    size_t pos = _requestString.find("\r\n\r\n");
    _requestString.erase(0, pos + 4);

    return _requestString;
}

void Client::setContentLength(size_t size)
{
    _contentLength = size;
}

size_t Client::getContentLength()
{
    return _contentLength;
}

void       Client::setMimeType(string mimeType)
{
    _mimeType = mimeType;
}

string     Client::getMimeType()
{
    return _mimeType;
}

void Client::setBody()
{
    size_t index = _requestString.find("\r\n\r\n");
    if (index == string::npos)
        return;
    _body = _requestString.substr(index + 4);
    _requestString.erase(0, index + 4);
}

double Client::getTimeout()
{
    return _timeout;
}


void        Client::receiveHeader()
{
    if (getStatus() == REDING && _requestString.find(ENDL ENDL) != string::npos)
    {
        this->setStatus(HEADER_RECEIVED);
        setHeaderReceivedVar(true);
    }
}

void       Client::setRequestHeader(map<string, string> request)
{
    _header_map = request;
    setHeaderReceivedVar(false);
}

void        Client::setRequest(Request request)
{
    _request = request;
}

Request        Client::getRequest()
{
    return _request;
}

void        Client::setIsUploading(bool isUploading)
{
    _isUploading = isUploading;
}

bool        Client::isUploading()
{
    return _isUploading;
}
