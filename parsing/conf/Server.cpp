/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 21:13:42 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/03/09 01:08:05 by yoelhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Location.hpp"


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
        this->_locations = copy._locations;
    }

    return *this;
}

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

void Server::setLocation(string path,Location locations)
{
    this->_locations[path] = locations;
}

size_t Server::getLengthLocation() const
{
    return this->_locations.size();
}