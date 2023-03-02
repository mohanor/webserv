/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 20:07:11 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/03/03 00:32:29 by yoelhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string.h>
#include <vector>

#include "Http.hpp"

using namespace std;

class Server : public Http
{
    protected:
        short int 	_listen;
        string		_server_name;
        string		_host;
    
    public:
        Server();
        Server(size_t _cli_max_size , string _root , string _index, string _error_page , string _allowed_methods , bool _autoindex, short int _listen, string _server_name, string _host );
        Server(const Server &copy);
        Server &operator=(const Server &copy);
        ~Server();

        string	getServerName() const;
		string	getHost() const;
		size_t	getListen() const;
        void setServerName(string name){
            _server_name = name;
        }
        void setHost(string host){
            _host = host;
        }
        void setListen(size_t port){
            _listen = port;
        }
};
