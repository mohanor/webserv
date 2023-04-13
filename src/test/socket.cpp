/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 21:39:23 by matef             #+#    #+#             */
/*   Updated: 2023/04/13 02:34:19 by yoelhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"

MimeTypes   SocketClass::_mime;

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

bool SocketClass::bindSocket(int listener, SocketServer &serverToBind, short port)
{
    bzero(&serverToBind.address, sizeof(serverToBind.address));
    
    serverToBind.address.sin_family = AF_INET;

    serverToBind.address.sin_addr.s_addr = INADDR_ANY; //TODO: change to server ip address from config file (host)
    serverToBind.address.sin_port = htons(port);

    int opt = 1;
    // opt = setsockopt(listener, SOL_SOCKET, SO_NOSIGPIPE , &opt, sizeof(opt));
    opt = setsockopt(listener, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt));
    if (opt < 0)
    {
        cerr << "Error setting socket options" << std::endl;
        return false;
        //exit(EXIT_FAILURE);
    }
    
    int bind_result = ::bind(listener, (struct sockaddr*) &serverToBind.address, sizeof(serverToBind.address));
    if (bind_result < 0)
    {
        cerr << "Error binding socket" << std::endl;
        return false;
        // exit(EXIT_FAILURE);
    }
    return true;
}

bool SocketClass::listenSocket(int listener)
{
    if (listen(listener, SOMAXCONN) < 0)
    {
        cerr << "Failed to listen for incoming connections" << std::endl;
        return false;
        //exit(EXIT_FAILURE);
    }
    return true;
}

int SocketClass::sendFileInPackets(struct pollfd &fds)
{
    cout << __FILE__ << " " << __LINE__ << endl;
    if (_clients[fds.fd].getStatus() == READY_TO_SEND)
    {
        cout << __FILE__ << " " << __LINE__ << endl;
        string response;
        
        string mimeType;
        string comment;
        

        response = "HTTP/1.1 200 Ok" ENDL;
        response += "Content-Type: " + _clients[fds.fd].getMimeType() + ENDL;
        response += "Content-Length: " + to_string(_clients[fds.fd].getContentLength()) + ( ENDL ENDL );
        
        _clients[fds.fd].setStatus(SENDING);
        send(fds.fd, response.c_str(), response.size(), 0);
        cout << __FILE__ << " " << __LINE__ << endl;
        return 0;
    }
    cout << __FILE__ << " " << __LINE__ << endl;
    string packet = _clients[fds.fd].getPacket();
    send(fds.fd, packet.c_str(), packet.size(), 0);
    cout << __FILE__ << " " << __LINE__ << endl;
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

Server SocketClass::getServer2(string host)
{

    short port = atoi(Request::getVector(host, ':')[1].c_str());
    string hostName = Request::getVector(host, ':')[0];

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
            cerr << "************************DELETE method" << endl;
            exit(0);
        }
    }

    return true;
}


bool SocketClass::isNewConnection(int listener)
{
    // Host: 127.0.0.1:8080
    // Host: webserver.com:8080

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
    // vector<string> hosts = server.getHost();
    (void) host;
    (void) server;

    // for (vector<string>::iterator it = hosts.begin(); it != hosts.end(); it++)
    // {
    //     if (*it == host)
    //         return true;
    // }

    return false;
}

bool SocketClass::isSeverNameBelongToServer(Server server, string serverName)
{
    return serverName == server.getServerName();
}


void SocketClass::setFds()
{
    int listener;
    bool bind, listen;

    for (vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
    {
        

        vector<short> ports = it->getPort();

        for (vector<short>::iterator pt = ports.begin(); pt != ports.end(); pt++)
        {
            cout << "port: " << *pt << endl;

            listener = this->create();
            if (listener < 0) continue;

            _s.push_back(SocketServer(listener, *it));

            bind = this->bindSocket(listener, _s[_s.size() - 1], *pt);
            if (!bind)
            {
                close(listener);
                _s.pop_back();
                continue;
            }

            listen = this->listenSocket(listener);
            if (!listen) continue;

            _fds.push_back(createPollfd(listener));
        }
        

    }

    for (vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
    {
        vector<short> ports = it->getPort();
        
        for(size_t i = 0; i < ports.size(); i++)
        {
            cerr << "http://localhost:" << ports[i] << endl;
        }
    }

    cout << "_fds: " << _fds.size() << endl;
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

    string host = _clients[fd]._request.getValueOf("Host");
    Method method = worker.getMethodObject(_clients[fd]._request, getServer2(host));
    _clients[fd].setFileContent(method.getResponse());
    _clients[fd].setStatus(READY_TO_SEND);
    _clients[fd].setContentLength(method.getResponse().size());
    _clients[fd].setMimeType(method.getContentType());
}

void SocketClass::createNewClient(int i)
{
    int newClient;
    
    newClient = accept(_s[i].sockfd, (struct sockaddr*)&_s[i].address, (socklen_t*)&_s[i].addrlen);
    if (newClient < 0)
    {
        cout << __FILE__ << " " << __LINE__ << endl;
        cerr << "fail to accept connection" << endl;
        return;
    }
    
    _serverHandler.insert(make_pair(newClient, _fds[i].fd));
    _fds.push_back(createPollfd(newClient));
}

void SocketClass::run()
{
    size_t     current_size;
    int       newClient;
    setFds();
    
    while (true)
    {
        cout << __FILE__ << " " << __LINE__ << endl;
        // cout << "listen ..." << endl;
        if (poll(&_fds[0], _fds.size(), -1) < 0)
        {
            cerr << "poll error" << endl;
            break;
        }
        cout << __FILE__ << " " << __LINE__ << endl;
        current_size = _fds.size();
        for(size_t i = 0; i < current_size; i++)
        {
            if (_fds[i].revents == 0)
                continue;
            cout << __FILE__ << " " << __LINE__ << endl;
            if (_fds[i].revents & POLLHUP)
            {
                cout << __FILE__ << " " << __LINE__ << endl;
                closeConnection(_fds[i].fd, i);
                current_size--;
                i--;
            }
            else if (_fds[i].revents & POLLIN)
            {   
                cout << __FILE__ << " " << __LINE__ << endl;
                if (isNewConnection(_fds[i].fd))
                {
                        newClient = accept(_s[i].sockfd, (struct sockaddr*)&_s[i].address, (socklen_t*)&_s[i].addrlen);
                        if (newClient < 0)
                        {
                            cerr << "fail to accept connection" << endl;
                            continue;
                        }
                        
                        _serverHandler.insert(make_pair(newClient, _fds[i].fd));
                        _fds.push_back(createPollfd(newClient));
                    // createNewClient(_fds[i].fd);
                    // continue;
                }
                else if ( !communicate(_fds[i]) )
                    break;
            }
            else if (_fds[i].revents & POLLOUT)
            {
                cout << __FILE__ << " " << __LINE__ << endl;
                if (_clients[_fds[i].fd].getStatus() == FILE_NOT_SET)
                    initResponse(_fds[i].fd);
                cout << __FILE__ << " " << __LINE__ << endl;
                sendFileInPackets(_fds[i]);
                cout << __FILE__ << " " << __LINE__ << endl;
                if (_clients[_fds[i].fd].getStatus() == SENDED)
                {
                    closeConnection(_fds[i].fd, i);
                    current_size--;
                    i--;
                }
                cout << __FILE__ << " " << __LINE__ << endl;
            }
        }

    }
cout << __FILE__ << " " << __LINE__ << endl;
}
