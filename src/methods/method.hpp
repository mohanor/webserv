/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 18:25:16 by yel-khad          #+#    #+#             */
/*   Updated: 2023/04/08 01:39:01 by yoelhaim         ###   ########.fr       */
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
# include "../utility/utility.hpp"
# include "../cgi/cgi.hpp"
# include "../../parsing/mime/MimeTypes.hpp"



using namespace std;

class Method 
{
    protected:
        Request _request;
        Server  _server;
        string  _resp;
        int     _status;
        string  _comment;
        string  _url;
        MimeTypes _mime;
        string  _contentType;

        map<string, Header> headers;
    public:
        Method(Request request, Server server);
        Method(int status, string comment, string url, string path, Request request, Server server);
        bool getRequestedResource();
        bool isFile();
        bool isDir();
        bool hasSlashInTheEnd();
        bool hasIndexFile();
        bool getAutoIndex();
        bool hasCGI();
        int getStatus() const;
        string getComment() const;
        string getURL() const;
        string getResponse() const;
        string getIndex();
        string getContentType();

};

class Post : public Method
{
    public:
        Post(Request request, Server server);
        bool locationSupportUpload();
};


string join_path(string s1, string s2);

#endif
