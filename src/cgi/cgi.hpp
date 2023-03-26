#pragma once 

#include "../methods/method.hpp"
#include <unistd.h>
using namespace std;

class CGI
{
    private:
        vector<string>  _env;
        string          _body;

    public:
        CGI(Request request, Server server,string body);
        char **_envToChar();

};