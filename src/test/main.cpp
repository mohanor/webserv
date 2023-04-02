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
#include "Client.hpp"

#include "../../parsing/request/Request.hpp"

using namespace std;

    



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


    // if (!file.is_open())
    // {
    //     cout << "file not open" << endl;
    //     return 1;
    // }

    // test(&file);
   
    // Client c;

    // c.setFile(av[1]);

    // SocketClass socket;
    string req = "POST / HTTP/1.1\r\n";

    req += "Transfer-Encoding: chunked\r\n";

    req += "\r\n";
    req += "5vnbnlgbmfg;l;\r\n";
    Request r(Request::deserialize(req));

    r.setBody("5vnbnlgbmfg;l;");
    cout << r.getBody() << endl;

    
    // Configuration config("./conf/default.conf");
    // vector<Server> server = config.getServers();
    // runCode(server[0], r);
   


    // cout << c.getPacket() << endl;
    // SocketClass s;
    // s.run();

    return 0;
}