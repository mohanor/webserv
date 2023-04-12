/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 00:23:47 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/04/12 02:51:43 by yoelhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
using namespace std;

#define ROOTPAGE "./var/www/html"

struct directives
{
    size_t cli_max_size ;
    //  vector<HostInfo> _host_info ;
    vector<short int> listen ;
    string server_name ;
    vector<string> host ;
    string root ;
    string index ;
    vector<pair<int, string> > error_page;
    bool autoindex;
    string allow;
    string cgi_info_php;
    string cgi_info_py;
    

    directives()
    {
        cli_max_size = 1000;
        // listen.push_back(8080);
        server_name = "localhost";
        // host.push_back("127.0.0.1");
        root = "./www/html";
        autoindex = false;
        allow = "GET POST DELETE";
    }
};


enum e_token
{
    OPEN_CURLY,
    CLOSE_CURLY,
    SEMI_COLON,
    WORD,
    DIRECTIVE,
    CONTEXT,
    WSPACE,
    NEWLINE,
   
};

class Http
{
protected:
    size_t _cli_max_size;
    string _root;
    string _index;
    vector<pair<int, string> > _error_page;
    string _allowed_methods;
    bool _autoindex;

public:
    Http(directives dir = directives());
    Http(const Http &copy);
    Http &operator=(const Http &copy);
    ~Http();

    size_t getMaxSize() const;
    string getRoot() const;
    string getIndex() const;
    vector<pair<int, string> > getErrorPage() const;
    string getAllowedMethods(int index) const;
    bool getAutoIndex() const;

};