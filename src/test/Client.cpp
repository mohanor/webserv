/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 03:41:11 by matef             #+#    #+#             */
/*   Updated: 2023/03/24 19:36:01 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"


Client::Client()
{
    _status = READY_TO_SEND;
    _timeout = TIMEOUT;
    _fileContent = "";
}

Client::Client(string fileContent)
{
    _status = READY_TO_SEND;
    _timeout = TIMEOUT;
    _fileContent = fileContent;
}

Client::Client(int fd)
{
    _status = FILE_NOT_SET;
    _timeout = TIMEOUT;
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

string Client::getFileContent()
{
    return _fileContent;
}

// void Client::setFile(string file)
// {
//     cout << "set file " << file << endl;
    
//     this->_file = new ifstream(file);

//     if (!(*_file).is_open())
//     {
//         cout << file << " file not open" << endl;
//         // _status = ERROR;
//         return;
//     }
//     _status = READY_TO_SEND;
// }

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
    _fileContent = _fileContent.substr(PACKET_SIZE, _fileContent.size());

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

double Client::getTimeout()
{
    return _timeout;
}




