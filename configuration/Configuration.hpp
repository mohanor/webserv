/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 22:02:15 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/03/05 21:17:08 by yoelhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "../parsing/conf/Server.hpp"

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
};

class Configuration
{
private:
    vector<Server> _server;
    vector<pair<string, int> > _tokens;
    vector<pair<string, int> > _directive_server;
    size_t _length_server;

public:
    Configuration();
    Configuration(vector<pair<string, int> > _tokens, size_t length);
    ~Configuration();
    // ??Check and  push to Server
    void insertServer();
    void addToServer();
    void checkDirective(size_t index);
    void checkLocation();
    void pushLocation(size_t index, size_t indexServer);
    size_t getDirectiveLocation(size_t i , size_t indexServer);

    void infoServer();
};