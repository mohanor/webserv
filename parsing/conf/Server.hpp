/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 20:07:11 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/04/13 21:33:13 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string.h>
#include <map>
#include <vector>


#include "Http.hpp"

using namespace std;

struct Location;
// struct HostInfo
// {
//     short port;
//     string hostName;
// };
//         vector<HostInfo>	        _host_info;

class Server : public Http
{
    protected:
        vector<short int>	             _listen;
        string		                _server_name;
        string	                     _host;
        string		                _cgi_info_php;
        string		                _cgi_info_py;
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
		vector<short int>	getPort() const;
        string  getCGIINFO() const;
        size_t getLengthLocation() const;
        string getMatchedLocation() const;
        string getCgiInfoPHP() const;
        string getCgiInfoPY() const;
        void setPort(short int port);
        
};
