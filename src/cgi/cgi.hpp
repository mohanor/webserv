/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-khad <yel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 06:37:51 by yel-khad          #+#    #+#             */
/*   Updated: 2023/04/10 02:39:12 by yel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 

#include "../methods/method.hpp"
#include <unistd.h>
#include <algorithm>
using namespace std;

class CGI
{
    private:
        Request         _request;
        Server          _server;
        string          _url;
        string          _method;
        vector<string>  _env;
        string          _resp;
        string          _scriptName;
        char **_args;

    public:
        CGI(Request request, Server server,string url, string method);
        char    **_envToChar();
        string  getResp();
        char    **setENV();
        void    getScriptName();
        bool    isPython();

};