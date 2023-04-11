/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 21:39:23 by matef             #+#    #+#             */
/*   Updated: 2023/04/11 21:30:06 by matef            ###   ########.fr       */
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
        // exit(EXIT_FAILURE);
    }

    return listener;
}

void SocketClass::bindSocket(int listener, SocketServer &serverToBind)
{
    bzero(&serverToBind.address, sizeof(serverToBind.address));
    
    serverToBind.address.sin_family = AF_INET;
    serverToBind.address.sin_addr.s_addr = INADDR_ANY;
    serverToBind.address.sin_port = htons(serverToBind.server.getPort());

    int opt = 1;
    // opt = setsockopt(listener, SOL_SOCKET, SO_NOSIGPIPE , &opt, sizeof(opt));
    opt = setsockopt(listener, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt));
    if (opt < 0)
    {
        cerr << "Error setting socket options" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    int bind_result = ::bind(listener, (struct sockaddr*) &serverToBind.address, sizeof(serverToBind.address));
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

int SocketClass::sendFileInPackets(struct pollfd &fds)
{
    // cout << "sendFileInPacket" << endl;
    // if (_clients[fds.fd].getStatus() == FILE_NOT_SET)
    // {
    //     _clients[fds.fd].setFileContent(file);
    //     _clients[fds.fd].setStatus(READY_TO_SEND);
    // }
    if (_clients[fds.fd].getStatus() == READY_TO_SEND)
    {
        string response;
        
        string mimeType;
        int status;
        string comment;
        

        response = "HTTP/1.1 200 Ok" ENDL;
        response += "Content-Type: " + _clients[fds.fd].getMimeType() + ENDL;
        response += "Content-Length: " + to_string(_clients[fds.fd].getContentLength()) + ( ENDL ENDL );
        
        _clients[fds.fd].setStatus(SENDING);
        send(fds.fd, response.c_str(), response.size(), 0);

        return 0;
    }
    string packet = _clients[fds.fd].getPacket();
    send(fds.fd, packet.c_str(), packet.size(), 0);
    return 0;
}

string SocketClass::joinRootAndPath(string root, string path, Request &httpRequest) //TODO : should be removed be cause i don't need it anymore
{
    if (httpRequest.getRessource() == "/")
        return "./www/html/index.html";

    return "./www/html" + httpRequest.getRessource();
}

bool SocketClass::isHeaderReceived(string request)
{
    return (request.find(ENDL ENDL) != string::npos);
}

bool SocketClass::recvError(int size, int fd)
{
    if ( size < 0 )
    {
        cerr << "recv failed" << endl;
        close(fd);
        return false;
    }
    else if ( !size )
    {
        cerr << "Connection closed" << endl;
        close(fd);
        return false;
    }
    return true;
}

unsigned long SocketClass::hex2dec(string hex) 
{
    unsigned long result = 0;

    for (int i=0; i<hex.length(); i++) {
        if (hex[i]>=48 && hex[i]<=57)
        {
            result += (hex[i]-48)*pow(16,hex.length()-i-1);
        } else if (hex[i]>=65 && hex[i]<=70) {
            result += (hex[i]-55)*pow(16,hex.length( )-i-1);
        } else if (hex[i]>=97 && hex[i]<=102) {
            result += (hex[i]-87)*pow(16,hex.length()-i-1);
        }
    }
    return result;
}

string SocketClass::parseChunked(string body)
{
    size_t pos = 0;
    string chunked;
    string size;
    bool isSize = true;
    bool firstLine = true;
    
    string prasedBody;

    while (body.size())
    {
        if (firstLine)
        {
            pos = body.find(ENDL);
            size = body.substr(0, pos);
            body.erase(0, pos + 2);
            firstLine = false;
            if (size == "0") break;
            continue;
        }
        
        prasedBody += body.substr(0, hex2dec(size));
        body.erase(0, hex2dec(size) + 2);
        firstLine = true;
    }

    return prasedBody;
}

bool SocketClass::handlePostRequest(Client &client)
{
    if (client._request.isHeaderHasKey("Content-Length"))
    {
        size_t contentLength = (size_t)atof( client._request.getValueOf("Content-Length").c_str() );

        if (contentLength == client.getReceivedLength() && client._request.isHeaderHasKey("Content-Type"))
        {

            vector <string> contentType = Request::getVector(client._request.getValueOf("Content-Type"));
            if (contentType[0] == "multipart/form-data;")
                client._request.setUploadable();
            

            // else case is not should handled
            
            client._request.setBody(client.getBody());
            client._requestString.clear();
            return true;
        }
    }
    else if (client._request.isHeaderHasKey("Transfer-Encoding"))
    {
        if (client._request.getValueOf("Transfer-Encoding") == "chunked")
        {
            if (client._requestString.rfind(ENDL "0" ENDL ENDL) != string::npos)
            {
                client.setBody();
                parseChunked(client.getBody());
                return true;
            }
        }
    }

    return false;
}

Server SocketClass::getServer(int sockfd)
{
    for(size_t i = 0; i < _s.size(); i++)
    {
        if (_s[i].sockfd == sockfd)
            return _s[i].server;
    }
    return _s[0].server; // return default server
}

void SocketClass::uploadFile(Request request)
{
    size_t nextLine ;
    string boundary;
    string endBoundary;
    string body;

    body = request.getBody();
    vector<string> contentType = Request::getVector(request.getValueOf("Content-Type"));
    
    boundary = "--" + contentType[1].substr(9);
    endBoundary = boundary + "--";


    size_t start = body.find(boundary);
    size_t end = body.find(endBoundary);

    if (start == string::npos || end == string::npos) // return msg error : bad request
    {
        cerr << "bad request" << endl;
        return;
    }
    
    string tmp;
    string bodyHead;
    vector <string> bodyHeadVector;
    
    while (start != string::npos && start < end)
    {
        nextLine = body.find("\r\n\r\n") + 4;
        bodyHead = body.substr(0, nextLine);

        while (bodyHead.find("\r\n") != string::npos)
            bodyHead.replace(bodyHead.find("\r\n"), 2, " ");

        bodyHeadVector = Request::getVector(bodyHead);
        while (bodyHeadVector.size() && bodyHeadVector[0].find("filename") == string::npos)
            bodyHeadVector.erase(bodyHeadVector.begin());

        if (!bodyHeadVector.size())
            return ;
        
        string filename = bodyHeadVector[0].substr(bodyHeadVector[0].find("\"") + 1);
        filename.erase(filename.find("\""));

        body.erase(0, nextLine);
        start = body.find(boundary);
        tmp = body.substr(0, start);

        ofstream file("./uploads/" + filename, ios::out | ios::trunc);

        file << tmp;
        tmp.clear();
    }
}

int SocketClass::communicate(struct pollfd &fds)
{
    string			req;
    size_t			size;
    char			request[MAX_REQUEST_SIZE] = {0};

    size = recv(fds.fd, request, MAX_REQUEST_SIZE, 0);
    if ( !recvError(size, fds.fd) ) return 0;

    if (_clients.find(fds.fd) == _clients.end())
    {
        Client client(fds.fd);
        _clients.insert(make_pair(fds.fd, client));
    }

    for(size_t i = 0; i < size; i++) _clients[fds.fd]._requestString.push_back(request[i]);

    _clients[fds.fd].receiveHeader();

    if (_clients[fds.fd].getStatus() == HEADER_RECEIVED)
    {
        if ( _clients[fds.fd].isHeaderRecived() )
        {
            _clients[fds.fd]._request = Request::deserialize(_clients[fds.fd].getHeader());

            int isOk = _clients[fds.fd]._request.isReqWellFormed();
            if (isOk != 200)
            {
                cout << "Request not well formed >> " << _clients[fds.fd]._request.getMethod() << " <<" << endl;
                cerr << "Request not well formed >> " << isOk << " <<" << endl;
                return (true);
            }

            _clients[fds.fd].setHeaderReceivedVar(false);
        }

        // else if (_clients[fds.fd].isHeaderRecived()) _clients[fds.fd].setRequest(httpRequest); // set request without body

        if (_clients[fds.fd].getRequest().getMethod() == POST)
        {
            cerr << "************************POST method" << endl;
            if (handlePostRequest(_clients[fds.fd]))
            {
                _clients[fds.fd].setStatus(FILE_NOT_SET);
                fds.events = POLLOUT;
            }
            return (true);
        }

        if (_clients[fds.fd].getRequest().getMethod() == GET)
        {
            cout << "************************GET method" << endl;
            _clients[fds.fd].setStatus(FILE_NOT_SET);
            fds.events = POLLOUT;
            return (true);
        }

        if (_clients[fds.fd].getRequest().getMethod() == DELETE)
        {
            cerr << "************************DELETE method" << endl;
            exit(0);
        }
    }

    return true;
}


bool SocketClass::isNewConnection(int listener)
{
    // cerr << "NewConnection" << endl;
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
        
        _s.push_back(SocketServer(listener, *it));

        this->bindSocket(listener, _s[_s.size() - 1]);
        this->listenSocket(listener);
        _fds.push_back(createPollfd(listener));

    }

    for (vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
        cerr << "http://localhost:" << it->getPort() << endl;
    cout << "_fds: " << _fds.size() << endl;
    for(size_t i = 0; i < _fds.size(); i++)
        cout << _fds[i].fd << endl;
}

void    SocketClass::closeConnection(int fd, int i)
{
    close(fd);
    _clients.erase(fd);
    _fds.erase(_fds.begin() + i);
}

void    SocketClass::initResponse(int fd)
{
    Worker worker;

    Method method = worker.getMethodObject(_clients[fd]._request, getServer(_serverHandler[fd]));
    _clients[fd].setFileContent(method.getResponse());
    _clients[fd].setStatus(READY_TO_SEND);
    _clients[fd].setContentLength(method.getResponse().size());
    _clients[fd].setMimeType(method.getContentType());
}

void SocketClass::run()
{
    size_t     current_size;
    int     newClient;

    setFds();
    
    while (true)
    {
        // cout << "listen ..." << endl;
        if (poll(&_fds[0], _fds.size(), -1) < 0)
        {
            cerr << "poll error" << endl;
            break;
        }

        current_size = _fds.size();
        for(int i = 0; i < current_size; i++)
        {
            if (_fds[i].revents == 0)
                continue;

            if (_fds[i].revents & POLLHUP)
            {
                closeConnection(_fds[i].fd, i);
                current_size--;
                i--;
            }
            else if (_fds[i].revents & POLLIN)
            {   
                if (isNewConnection(_fds[i].fd))
                {
                    newClient = accept(_s[i].sockfd, (struct sockaddr*)&_s[i].address, (socklen_t*)&_s[i].addrlen);
                    if (newClient < 0) { cerr << "fail to accept connection" << '\n'; continue; }
                    _serverHandler.insert(make_pair(newClient, _fds[i].fd));
                    _fds.push_back(createPollfd(newClient));
                }
                else if ( !communicate(_fds[i]) )
                    break;
            }
            else if (_fds[i].revents & POLLOUT)
            {
                if (_clients[_fds[i].fd].getStatus() == FILE_NOT_SET)
                    initResponse(_fds[i].fd);

                sendFileInPackets(_fds[i]);
                
                if (_clients[_fds[i].fd].getStatus() == SENDED)
                {
                    closeConnection(_fds[i].fd, i);
                    current_size--;
                    i--;
                }
            }
        }

    }

}
