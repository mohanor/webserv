/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 20:07:11 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/03/07 15:51:36 by yoelhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string.h>
#include <map>
#include <vector>


#include "Http.hpp"

using namespace std;
class Location;

class Server : public Http
{
    protected:
        short int 	                _listen;
        string		                _server_name;
        string		                _host;
        string		                _length_location;

    public:
        map<string , Location> _locations;
        vector<pair<string, int> > _location;
        Server(directives dir);
        
        Server(const Server &copy);
        Server &operator=(const Server &copy);
        ~Server();

        string	getServerName() const;
		string	getHost() const;
		size_t	getListen() const;
};
