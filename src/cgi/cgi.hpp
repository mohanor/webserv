#pragma once 

#include "../methods/method.hpp"
#include <unistd.h>
#include <algorithm>
using namespace std;

class CGI
{
    private:
        vector<string>  _env;
        string          _body;
        string          _scriptName;

    public:
        CGI(Request request, Server server,string url);
        char **_envToChar();
        char **getScriptName(Request request, Server server, string url);

};