/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 21:39:23 by matef             #+#    #+#             */
/*   Updated: 2023/04/18 01:15:41 by matef            ###   ########.fr       */
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
        {
            cout << "Location: " + _clients[fds.fd].getReturn() << endl;
            response += "Location: " + _clients[fds.fd].getReturn() + ENDL;
        }

        map<string, string> cgiHeader = _clients[fds.fd].getCgiHeader();
        
        for (map<string, string>::iterator it = cgiHeader.begin(); it != cgiHeader.end(); it++)
        {
            response += it->first + ": " + it->second + ENDL;
        }
         
        response += ENDL;
        _clients[fds.fd].setStatus(SENDING);
        
        send(fds.fd, response.c_str(), response.size(), 0);
        return 0;
    }

    string packet = _clients[fds.fd].getPacket();
    send(fds.fd, packet.c_str(), packet.size(), 0);
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

unsigned long SocketClass::hex2dec(string hex) 
{
    unsigned long result = 0;

    for (size_t i=0; i<hex.length(); i++) {
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

bool SocketClass::handleDeleteRequest(Client &client)
{
    if (client._request.isHeaderHasKey("Content-Length"))
    {
        size_t contentLength = (size_t)atof( client._request.getValueOf("Content-Length").c_str() );

        if (contentLength == client.getReceivedLength() && client._request.isHeaderHasKey("Content-Type"))
        {
            client._request.setBody(client.getBody());
            client._requestString.clear();
            return true;
        }
    }

    // chunked case
    
    return false;
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
                
                cout << client._requestString << endl;
                exit(0);
                client.setBody();
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
                _clients[fds.fd].setComment("hello");
                return (false);
            }

            _clients[fds.fd].setHeaderReceivedVar(false);
        }

        if (_clients[fds.fd].getRequest().getMethod() == POST)
        {
            // cout << "POST method" << endl;
            
            if (handlePostRequest(_clients[fds.fd]))
            {
                _clients[fds.fd].setStatus(FILE_NOT_SET);
                fds.events = POLLOUT;
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
            if (handleDeleteRequest(_clients[fds.fd]))
            {
                _clients[fds.fd].setStatus(FILE_NOT_SET);
                fds.events = POLLOUT;
            }
            return (true);
        }
    }

    return true;
}


bool SocketClass::isNewConnection(int listener)
{
    // cout << "is new connection: " << _s.size() << endl;

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

    cout << "_fds: " << _fds.size() << endl;
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
    cout << __LINE__ << " " << __FILE__ << endl;
    Worker worker;
    cout << __LINE__ << " " << __FILE__ << endl;
    string host = _clients[fd]._request.getValueOf("Host");
    cout << __LINE__ << " " << __FILE__ << endl;
    Method method = worker.getMethodObject(_clients[fd]._request, getServer2(host));
    cout << __LINE__ << " " << __FILE__ << endl;
    _clients[fd].setFileContent(method.getResponse());
    _clients[fd].setStatus(READY_TO_SEND);
    _clients[fd].setContentLength(method.getResponse().size());
    _clients[fd].setMimeType(method.getContentType());
    _clients[fd].setRespStatus(to_string(method.getStatus()));
    _clients[fd].setComment(method.getComment());
    _clients[fd].setReturn(method.getRedirection());
    _clients[fd].setCgiHeader(method.getHeaders());
    cout << __LINE__ << " " << __FILE__ << endl;
    
}

void SocketClass::sendErrorReply(int i)
{
    cout << __LINE__ << " " << __FILE__ << endl;
    string status = _clients[_fds[i].fd].getRespStatus();
    string pageErrorUrl;
    string page;
    
    pageErrorUrl = getServer(_fds[i].fd).getErrorPageByIndex(atoi(status.c_str()));
    if (pageErrorUrl == "")
        pageErrorUrl = "./error_page/" + status + ".html";
    
    cout << "pageErrorUrl: " << pageErrorUrl << endl;
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
        // cout << "listening..." << endl;
        if (poll(&_fds[0], _fds.size(), -1) < 0)
        {
            cerr << "poll error" << endl;
            break;
        }
        cout << __LINE__ << " " << __FILE__ << endl;
        for(size_t i = 0; i < _fds.size(); i++)
        {
            if (_fds[i].revents & POLLHUP)
            {
                closeConnection(i);
            }
            else if (_fds[i].revents & POLLIN)
            {
                cout << __LINE__ << " " << __FILE__ << endl;
                if (isNewConnection(_fds[i].fd))
                {
                    cout << __LINE__ << " " << __FILE__ << endl;
                    newClient = accept(_s[i].sockfd, NULL, NULL);
                    cout << "new client: " << newClient << endl;
                    if (newClient < 0) { cerr << "fail to accept connection" << endl; continue; }
                    opt = setsockopt(newClient, SOL_SOCKET, SO_NOSIGPIPE , &opt, sizeof(opt));
                    struct pollfd pfd = {newClient, POLLIN, 0};
                    _fds.push_back(pfd);
                    
                }
                else if ( !communicate(_fds[i]) )
                {
                    // _clients[_fds[i].fd].getRespStatus();
                    // _clients[_fds[i].fd].getComment();
                    cout << __LINE__ << " " << __FILE__ << endl;
                    
                    sendErrorReply(i);
                    closeConnection(i);
                    continue;
                }
            }
            else if (_fds[i].revents & POLLOUT)
            {
               cout << __LINE__ << " " << __FILE__ << endl;
                if (_clients[_fds[i].fd].getStatus() == FILE_NOT_SET) initResponse(_fds[i].fd);
                cout << __LINE__ << " " << __FILE__ << endl;
                sendFileInPackets(_fds[i]);
                cout << __LINE__ << " " << __FILE__ << endl;
                if (_clients[_fds[i].fd].getStatus() == SENDED)
                {
                    closeConnection(i);
                }
            }
        }
    }
}
