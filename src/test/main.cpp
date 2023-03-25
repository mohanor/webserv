#include <iostream>
#include <fstream>
#include "../../parsing/conf/ConfigParser.hpp"
#include "../../configuration/Configuration.hpp"
#include "../utility/utility.hpp"

#include "../../parsing/request/Request.hpp"

#include "../../parsing/mime/MimeTypes.hpp"
#include "../methods/delete.hpp"
#include "../server/Worker.hpp"

#include "socket.hpp"


using namespace std;

template <typename T>
T test(T a, T b)
{
   return  a + b;
}


void runCode(Server &server, Request req)
{
     Worker worker;
  
    if (worker.getMatchedLocationFoRequestUri(req.getRessource(), server))
    {
        if (worker.isLocationHaveRedirection(server))
        {
            cout << "redirection" << endl;
            return;
        }
        if (!worker.isMethodAllowdedInLocation("DELETE",server))
        {
            cout << "dont allowed" << endl;
            return;
        }
    }
    else
     return;
    Delete del(req, server);
   
}

int main(int ac, char **av)
{
    SocketClass socket;
    string req = "DELETE /correction/hi/ HTTP/1.1\r\n";

    req += "Host: localhost:8080\r\n";

    req += "\r\n";
    Request r(Request::deserialize(req));


    
    Configuration config("./conf/default.conf");
    vector<Server> server = config.getServers();
    runCode(server[0], r);
   



    return 0;
}