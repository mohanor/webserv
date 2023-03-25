/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-khad <yel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 18:25:16 by yel-khad          #+#    #+#             */
/*   Updated: 2023/03/25 03:47:38 by yel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef METHOD_HPP
# define METHOD_HPP

# include <iostream>
# include <dirent.h>
# include <stdio.h>
# include "../../parsing/request/Request.hpp"
# include "../../parsing/conf/Server.hpp"
# include "../../parsing/conf/Location.hpp"


using namespace std;

class Method 
{
    protected:
        Request _request;
        Server  _server;
        string  _path;
        int     _status;
        string  _comment;
        string  _url;

        map<string, Header> headers;
    public:
        Method(Request request, Server server);
        bool getRequestedResource();
        bool isFile();
        bool isDir();
        bool hasSlashInTheEnd();
        bool hasIndexFile();
        bool getAutoIndex();
        bool hasCGI();
        string getIndex();
};

class Post : public Method
{
    public:
        Post(Request request, Server server);
        bool locationSupportUpload();
};


string join_path(string s1, string s2);

#endif
