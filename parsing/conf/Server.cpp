/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 21:13:42 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/03/05 21:29:19 by yoelhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"



Server::Server(directives dir) : Http(dir)
{
    this->_server_name = dir.server_name;
    this->_host = dir.host;
    this->_listen = dir.listen;
}

Server::~Server() {}

Server::Server(const Server &copy) :Http(copy)
{
    *this = copy;
}

Server &Server::operator=(const Server &copy)
{
    if (this != &copy)
    {
        this->_server_name = copy._server_name;
        this->_host = copy._host;
        this->_listen = copy._listen;
    }

    return *this;
}

/*
    void Server::setServerName(string name)
    {
        _server_name = name;
    }
    void Server::setHost(string host)
    {
        _host = host;
    }
    void Server::setMaxSize(size_t size)
    {
        _cli_max_size = size;
    }
    void Server::setListen(size_t port)
    {
        _listen = port;
    }
    void Server::setRoot(string root)
    {
        _root = root;
    }
    void Server::setIndex(string index)
    {
        _index = index;
    }
    void Server::setErrorPage(string path[2])
    {
    }
    void Server::setAutoIndex(bool value)
{
    _autoindex = value;
}
*/

// ?? getter
string Server::getServerName() const
{
    return this->_server_name;
}
string Server::getHost() const
{
    return this->_host;
}

size_t Server::getListen() const { return this->_listen; }
