/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 21:39:23 by matef             #+#    #+#             */
/*   Updated: 2023/04/19 07:07:02 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"



SocketClass::SocketClass(string config_file)
{
    servers = Configuration(config_file).getServers();
}

SocketClass::~SocketClass()
{
}

int SocketClass::create()
{
    int listener;

    listener = socket(AF_INET, SOCK_STREAM, 0); // AF_INET = IPv4, SOCK_STREAM = TCP, 0 = default protocol of socket type
    if (listener < 0)
        cerr << "socket failed" << std::endl;

    return listener;
}

bool SocketClass::bindSocket(int listener, SocketServer &serverToBind, HostAndPort hostAndPort)
{
    bzero(&serverToBind.address, sizeof(serverToBind.address));
    
    serverToBind.address.sin_family = AF_INET;

    serverToBind.address.sin_addr.s_addr =  inet_addr(hostAndPort.host.c_str());
    serverToBind.address.sin_port = htons(hostAndPort.port);
    int opt = 1;
    opt = setsockopt(listener, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt));
    if (opt < 0)
    {
        cerr << "Error setting socket options" << std::endl;
        return false;
    }
    
    int bind_result = ::bind(listener, (struct sockaddr*) &serverToBind.address, sizeof(serverToBind.address));
    if (bind_result < 0)
    {
        cerr << "Error binding socket" << std::endl;
        return false;
    }
    return true;
}

bool SocketClass::listenSocket(int listener)
{
    if (listen(listener, 1000) < 0)
    {
        cerr << "Failed to listen for incoming connections" << std::endl;
        return false;
    }
    return true;
}

int SocketClass::sendFileInPackets(struct pollfd &fds)
{

    if (_clients[fds.fd].getStatus() == READY_TO_SEND)
    {
        string response;
        string mimeType;
        string comment;

        response = "HTTP/1.1 " + _clients[fds.fd].getRespStatus() + " "  + _clients[fds.fd].getComment() + ENDL;
        response += "Content-Type: " + _clients[fds.fd].getMimeType() + ENDL;
        response += "Content-Length: " + to_string(_clients[fds.fd].getContentLength()) + ENDL;
        
        if (_clients[fds.fd].getReturn().size())
            response += "Location: " + _clients[fds.fd].getReturn() + ENDL;

        map<string, string> cgiHeader = _clients[fds.fd].getCgiHeader();
        for (map<string, string>::iterator it = cgiHeader.begin(); it != cgiHeader.end(); it++)
            response += it->first + ": " + it->second + ENDL;
         
        response += ENDL;
        _clients[fds.fd].setStatus(SENDING);
        
        int err = send(fds.fd, response.c_str(), response.size(), 0);
        if (err <= 0 ) close(fds.fd);
        
        return 0;
    }

    string packet = _clients[fds.fd].getPacket();
    int err = send(fds.fd, packet.c_str(), packet.size(), 0);
    if (err <= 0 ) close(fds.fd);
    return 0;
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

//TODO change this function
unsigned long SocketClass::hexToDec(string hex) 
{

    unsigned long result = 0;

    for (size_t i = 0; i < hex.length(); i++) 
    {
        if (hex[i] >= '0' && hex[i] <= '9')
            result += (hex[i] - 48) * pow(16, hex.length() - (i + 1));

        else if (hex[i] >= 'A' && hex[i] <= 'F')
            result += (hex[i] - 55) * pow(16, hex.length( ) - (i + 1));

        else if (hex[i] >= 'a' && hex[i] <= 'f')
            result += (hex[i] - 87) * pow(16, hex.length() - (i + 1));
    }
    return result;
}

string SocketClass::parseChunked(string body, int *c)
{
    size_t pos = 0;
    string chunked;
    string size;
    bool firstLine = true;
    
    string prasedBody;

    size_t p = body.find(ENDL ENDL);
    body.erase(0, p + 4);

    string temp;
    while (body.size())
    {
        if (firstLine)
        {
            pos = body.find(ENDL);
            size = body.substr(0, pos);
            if (!sizeIsHex(size))
            {
                *c = 1;
                return "";
            }

            size = to_string(hexToDec(size));
            body.erase(0, pos + 2);
            firstLine = false;
            if (size == "0") break;
            continue;
        }
        
        temp = body.substr(0, hexToDec(size));
        if (temp.size() != hexToDec(size))
        {
            *c = 1;
            return "";
        }
        prasedBody += temp;
        body.erase(0, hexToDec(size) + 2);
        firstLine = true;
    }

    return prasedBody;
}

int SocketClass::handleDeleteRequest(Client &client)
{
    int c = 0;
    if (client._request.isHeaderHasKey("Content-Length"))
    {
        size_t contentLength = (size_t)atof( client._request.getValueOf("Content-Length").c_str() );

        if (contentLength == client.getReceivedLength() && client._request.isHeaderHasKey("Content-Type"))
        {
            client._request.setBody(client.getBody());
            client._requestString.clear();
            return 1;
        }
    }
    else if (client._request.isHeaderHasKey("Transfer-Encoding"))
    {
        if (client._request.getValueOf("Transfer-Encoding") == "chunked")
        {
            if (client._requestString.rfind(ENDL "0" ENDL ENDL) != string::npos)
            {
                string body = parseChunked(client._requestString, &c);
                if (c)
                {
                    client._requestString.clear();
                    return 2;
                }
                client._request.setBody(body);
                client._requestString.clear();
                return 1;
            }
        }
    }
    
    return 0;
}

int SocketClass::handlePostRequest(Client &client)
{
    int c = 0;

    if (client._request.isHeaderHasKey("Content-Length"))
    {
        size_t contentLength = (size_t)atof( client._request.getValueOf("Content-Length").c_str() );

        if (contentLength == client.getReceivedLength() && client._request.isHeaderHasKey("Content-Type"))
        {
            vector <string> contentType = Request::getVector(client._request.getValueOf("Content-Type"));
            if (contentType.size() && contentType[0] == "multipart/form-data;")
                client._request.setUploadable();

            client._request.setBody(client.getBody());
            client._requestString.clear();
            return 1;
        }
    }
    else if (client._request.isHeaderHasKey("Transfer-Encoding"))
    {
        if (client._request.getValueOf("Transfer-Encoding") == "chunked")
        {
            if (client._requestString.rfind(ENDL "0" ENDL ENDL) != string::npos)
            {
                vector <string> contentType = Request::getVector(client._request.getValueOf("Content-Type"));
                if (contentType.size() && contentType[0] == "multipart/form-data;")
                    client._request.setUploadable();

                string parsedBody = parseChunked(client._requestString, &c);
                if (c)
                {
                    client._requestString.clear();
                    return 2;
                }
                client._request.setBody(parsedBody);
                client._requestString.clear();
                return 1;
            }
        }
    }

    return 0;
}

Server SocketClass::getServer(int sockfd)
{
    for(size_t i = 0; i < _s.size(); i++)
    {
        if (_s[i].sockfd == sockfd)
            return _s[i].server;
    }
    return _s[0].server;
}

Server SocketClass::getServer2(string host)
{
    
    vector<string> hostAndPort = Request::getVector(host, ':');

    if (hostAndPort.size() != 2) return _s[0].server;

    short port = atoi(hostAndPort[1].c_str());
    string hostName = hostAndPort[0];

    vector<SocketServer>::iterator serverWillserve = _s.begin();
    bool firstServer = true;

    for(vector<SocketServer>::iterator it = _s.begin()  ; it != _s.end(); it++)
    {
        if (isPortBelongToServer(it->server, port))
        {
            if (firstServer)
            {
                firstServer = false;
                serverWillserve = it;
            }

            if (isHostBelongToServer(it->server, hostName) || isSeverNameBelongToServer(it->server, hostName))
                return it->server;
        }
    }
    
    return serverWillserve->server;
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
                _clients[fds.fd].setRespStatus(to_string(isOk));
                _clients[fds.fd].setComment(getComment(isOk));
                return (false);
            }

            _clients[fds.fd].setHeaderReceivedVar(false);
        }

        if (_clients[fds.fd].getRequest().getMethod() == POST)
        {
            int handleReturn = handlePostRequest(_clients[fds.fd]);

            if ( handleReturn == 1 )
            {
                _clients[fds.fd].setStatus(FILE_NOT_SET);
                fds.events = POLLOUT;
            }
            else if ( handleReturn == 2 )
            {
                _clients[fds.fd].setRespStatus("400");
                _clients[fds.fd].setComment(getComment(400));
                return (false);
            }
            
            return (true);
        }

        if (_clients[fds.fd].getRequest().getMethod() == GET)
        {
            _clients[fds.fd].setStatus(FILE_NOT_SET);
            fds.events = POLLOUT;
            return (true);
        }

        if (_clients[fds.fd].getRequest().getMethod() == DELETE)
        {
            int handleReturn = handleDeleteRequest(_clients[fds.fd]);

            if ( handleReturn == 1 )
            {
                _clients[fds.fd].setStatus(FILE_NOT_SET);
                fds.events = POLLOUT;
            }
            else if ( handleReturn == 2 )
            {
                _clients[fds.fd].setRespStatus("400");
                _clients[fds.fd].setComment(getComment(400));
                return (false);
            }
            return (true);
        }
    }

    return true;
}


bool SocketClass::isNewConnection(int listener)
{
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
    pfd.revents = 0;
    return pfd;
}

bool SocketClass::isPortBelongToServer(Server server, short port)
{
    vector<short> ports = server.getPort();

    for (vector<short>::iterator it = ports.begin(); it != ports.end(); it++)
    {
        if (*it == port)
            return true;
    }

    return false;
}

bool SocketClass::isHostBelongToServer(Server server, string host)
{
    return host == server.getHost();
}

bool SocketClass::isSeverNameBelongToServer(Server server, string serverName)
{
    return serverName == server.getServerName();
}

bool isListen(vector<HostAndPort> hostsAndPorts, HostAndPort hostAndPort)
{
    for (vector<HostAndPort>::iterator it = hostsAndPorts.begin(); it != hostsAndPorts.end(); it++)
    {
        if (it->host == hostAndPort.host && it->port == hostAndPort.port)
            return true;
    }
    return false;
}

bool SocketClass::isAllPortDiffrents(Server s1, Server s2)
{
    vector<short> ports1 = s1.getPort();
    
    for (vector<short>::iterator it = ports1.begin(); it != ports1.end(); it++)
    {
        if (isPortBelongToServer(s2, *it))
            return false;
    }
    return true;
}

void SocketClass::setFds()
{
    int listener;
    bool bind, listen;

    vector<HostAndPort> hostsAndPorts;
    HostAndPort hostAndPort;


    for (vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
    {
        vector<short> ports = it->getPort();

        for (vector<short>::iterator pt = ports.begin(); pt != ports.end(); pt++)
        {
            cout << "port: " << *pt << endl;

            hostAndPort.host = it->getHost();
            hostAndPort.port = *pt;
            if (isListen(hostsAndPorts, hostAndPort))
                continue;
            listener = this->create();
            if (listener < 0) continue;

            _s.push_back(SocketServer(listener, *it));

            bind = this->bindSocket(listener, _s[_s.size() - 1], hostAndPort);
            if (!bind)
            {
                close(listener);
                _s.pop_back();
                continue;
            }

            listen = this->listenSocket(listener);
            if (!listen) continue;

            _fds.push_back(createPollfd(listener));
            hostsAndPorts.push_back(hostAndPort);
        }

    }

    if (_fds.size() == 0)
    {
        cerr << "server (s) resource alreday in use" << endl;
        exit (1);
    }
}

void    SocketClass::closeConnection(int i)
{
    close(_fds[i].fd);
    _clients.erase(_fds[i].fd);
    _fds.erase(_fds.begin() + i);
}

void    SocketClass::initResponse(int fd)
{

    Worker worker;
    string host = _clients[fd]._request.getValueOf("Host");
    Method method = worker.getMethodObject(_clients[fd]._request, getServer2(host));
    
    _clients[fd].setFileContent(method.getResponse());
    _clients[fd].setStatus(READY_TO_SEND);
    _clients[fd].setContentLength(method.getResponse().size());
    _clients[fd].setMimeType(method.getContentType());
    _clients[fd].setRespStatus(to_string(method.getStatus()));
    _clients[fd].setComment(method.getComment());
    _clients[fd].setReturn(method.getRedirection());
    _clients[fd].setCgiHeader(method.getHeaders());
    
}

void SocketClass::sendErrorReply(int i)
{
    string status = _clients[_fds[i].fd].getRespStatus();
    string pageErrorUrl;
    string page;
    
    pageErrorUrl = getServer(_fds[i].fd).getErrorPageByIndex(atoi(status.c_str()));
    if (pageErrorUrl == "")
        pageErrorUrl = "./error_page/" + status + ".html";
    
    page = getFileContent(pageErrorUrl);
    
    string error = "HTTP/1.1 " + status + " " + getComment(atoi(status.c_str())) + "\r\n";
    error += "Content-Type: text/html\r\n";
    error += "Content-Length: " + to_string(page.size()) + "\r\n";
    error += "\r\n";
    
    error += page;
    send(_fds[i].fd, error.c_str(), error.size(), 0);
}

void SocketClass::run()
{
    int       newClient;
    int       opt;

    setFds();
    while (true)
    {
        if (poll(&_fds[0], _fds.size(), -1) < 0)
        {
            cerr << "poll error" << endl;
            break;
        }
        for(size_t i = 0; i < _fds.size(); i++)
        {
            if (_fds[i].revents & POLLHUP)
                closeConnection(i);
            else if (_fds[i].revents & POLLIN)
            {
                if (isNewConnection(_fds[i].fd))
                {
                    newClient = accept(_s[i].sockfd, NULL, NULL);
                    if (newClient < 0) { cerr << "fail to accept connection" << endl; continue; }
                    opt = setsockopt(newClient, SOL_SOCKET, SO_NOSIGPIPE , &opt, sizeof(opt));
                    struct pollfd pfd = {newClient, POLLIN, 0};
                    _fds.push_back(pfd);
                }
                else if ( !communicate(_fds[i]) )
                {
                    sendErrorReply(i);
                    closeConnection(i);
                    continue;
                }
            }
            else if (_fds[i].revents & POLLOUT)
            {
                if (_clients[_fds[i].fd].getStatus() == FILE_NOT_SET) initResponse(_fds[i].fd);
                sendFileInPackets(_fds[i]);
                if (_clients[_fds[i].fd].getStatus() == SENDED)
                    closeConnection(i);
            }
        }
    }
}
