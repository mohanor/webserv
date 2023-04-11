/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-khad <yel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 18:25:16 by yel-khad          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/04/10 23:25:21 by yel-khad         ###   ########.fr       */
=======
/*   Updated: 2023/04/09 21:48:59 by yoelhaim         ###   ########.fr       */
>>>>>>> a53309f7479213fe4a59655d4e816dccdf3727b3
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
        map<int, string> _error_page;
    public:
        Method(Request request, Server server);
        Method(int status, string comment, string url, Request request, Server server);
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
        void insetErrorPage();

};
<<<<<<< HEAD



=======
>>>>>>> a53309f7479213fe4a59655d4e816dccdf3727b3
string join_path(string s1, string s2);

#endif
