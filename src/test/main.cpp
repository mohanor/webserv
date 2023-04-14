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

    


//TODO for youssef => include MIMETYPES in config file
MimeTypes _mime;

int main(int ac, char **av)
{

    (void)ac;
    (void)av;
    // SocketClass socket;
    // Worker worker;
    
    // string req = "GET /home/ HTTP/1.1\r\n";


    // req += "\r\n";

    // req += "librait\r\n";
    // Request r(Request::deserialize(req));

    // cout << "#" << r.getMethod() << "\n\r123456789" << endl;
    // int s = r.isReqWellFormed();
    
    // cout << s  << endl;
    // cout << r.getQueryString()<< endl;
    // r.setBody("12547");

    

    // Configuration config("./conf/default.conf");
    // vector<Server> server = config.getServers();
    // // runCode(server[0], r);


    // Configuration config("./conf/default.conf");
    // vector<Server> server = config.getServers();
    // cout << server.size() << endl;
    // Method m =  worker.getMethodObject(r, server[0]);

    // cout << "status : " << m.getStatus() << endl;
    // cout << "path   : " <<m.getResponse() << endl;
    // cout << "comme  : "<<m.getComment() << endl;
    // cout << "url  : "<<m.getURL() << endl;
    // cout << "---------------------" << endl;
    // worker.listenDirectory("/Users/yoelhaim/Desktop/webserv/www/html/") ;

    

// cout << getFileContent("./error_page/404.html") << endl ;

    SocketClass s;

    s.run();
    return 0;
}