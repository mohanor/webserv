/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-khad <yel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 18:25:16 by yel-khad          #+#    #+#             */
/*   Updated: 2023/04/15 01:32:20 by yel-khad         ###   ########.fr       */
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
#define STATUSCODE "201 301 400 401 403 404 405 408 413 414 500 501 502 503 504 505"



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
        string _redirection;
        // MimeTypes _mime;
        string  _contentType;

        map<string, string> _headers;
        map<int, string> _error_page;
    public:
        Method(Request request, Server server);
        Method(int status, string comment, string redirection, Request request, Server server);
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
        string getRedirection() const;
        void insetErrorPage();
        void deserialize();
        string getRidOfHeaders();
};


string join_path(string s1, string s2);

#endif
