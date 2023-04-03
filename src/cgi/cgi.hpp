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
        char **_envToChar();
        string getResp();
        char **setENV();
        void    getScriptName();

};