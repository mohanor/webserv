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

    SocketClass socket;
     Worker worker;
    string req = "DELETE /home/ HTTP/1.1\r\n";

    req += "Host: localhost:8080\r\n";

    req += "\r\n";
    Request r(Request::deserialize(req));


    
    Configuration config("./conf/default.conf");
    vector<Server> server = config.getServers();
    Method m =  worker.getMethodObject(r,server[0]);

    cout << "status : " << m.getStatus() << endl;
    cout << "path   : " <<m.getPath() << endl;
    cout << "comme  : "<<m.getComment() << endl;
    cout << "---------------------" << endl;
    worker.listenDirectory("/Users/yoelhaim/Desktop/webserv/www") ;

    
   



    // cout << c.getPacket() << endl;
    // SocketClass s;
    // s.run();

    return 0;
}