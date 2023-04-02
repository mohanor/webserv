/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-khad <yel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 20:07:11 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/03/31 00:35:57 by yel-khad         ###   ########.fr       */
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
        string		                _cgi_info;
        string		                _length_location;
        string		                _matched_location;

    public:
        map<string , Location >      _locations;
        vector<pair<string, int> > _location;
        Server(directives dir);
        
        Server(const Server &copy);
        Server &operator=(const Server &copy);
        ~Server();
        void setLocation(string path, Location locations);
        void setMatchedLocation(string location);

        string	getServerName() const;
		string	getHost() const;
		size_t	getPort() const;
        string  getCGIINFO() const;
        size_t getLengthLocation() const;
        string getMatchedLocation() const;
        string getCgiInfo() const;
        
};
