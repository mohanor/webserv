/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 22:02:15 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/04/10 00:20:17 by yoelhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "../parsing/conf/Server.hpp"
#include "../parsing/conf/ConfigParser.hpp"
#include "../parsing/conf/Location.hpp"

using namespace std;

enum directive
{
    LISTEN,
    SERVER_NAME,
    HOST,
    CLIENT_MAX_BODY_SIZE,
    ROOT,
    INDEX,
    ERROR_PAGE,
    ALLOWED_METHODS,
    AUTOINDEX,
    RETURN,
    UPLOAD_STORE,
    UPLOAD_ENABLE,
     CGI_INFO_PHP,
    CGI_INFO_PYTHON,};

class Configuration
{
private:
    vector<Server> _server;
    vector<pair<string, int> > _tokens;
    vector<pair<string, int> > _directive_server;
    size_t _length_server;
    vector< Location> _locations;
    ConfigParser _config;
   

public:
    Configuration(string fileName = "./conf/default.conf");
    ~Configuration();
    Configuration(const Configuration &copy);
    Configuration &operator=(const Configuration &copy);
    // ??Check and  push to Server
    void insertServer();
    void addToServer();
    void checkDirective(size_t index);
    void checkLocation();
    void checkDirectiveLocation(size_t index, size_t indexServer);
    size_t getDirectiveLocation(size_t i , size_t indexServer, string nameLocation);
    void pushLocation(size_t index, string nameLocation);
    string getKey(int index);
    vector <Server> getServers() const ;
    map<string , Location > getLocationServer();
    void infoServer();
};