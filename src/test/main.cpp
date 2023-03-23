#include <iostream>
#include <fstream>
#include "../../parsing/conf/ConfigParser.hpp"
#include "../../configuration/Configuration.hpp"
#include "../utility/utility.hpp"

#include "../../parsing/request/Request.hpp"

#include "../../parsing/mime/MimeTypes.hpp"

#include "socket.hpp"


using namespace std;

template <typename T>
T test(T a, T b)
{
   return  a + b;
}

int main(int ac, char **av)
{

    vector v;
    // int a;
    // SocketClass s;
    // s.run();
    // Request o("",av[1],"");
    // o.resource();

    // cout << o.getRessource() << endl;
    // Configuration conf("./conf/default.conf");

    // vector<Server> s = conf.getServers();

    // vector<string> v = Request::getVector("vf kvbdk fvbdfk vbdk", 'f');

    // for(int i = 0; i < s.size(); i++)
    // {
    //     cout << s[i].getHost() << endl;
    //     cout << s[i].getPort() << endl;
    // }


    // string file = "./conf/mime.types";

    // MimeTypes m;
    // m.parseMimeTypes(file);

    // cout << m.getMimeType("htm") << endl;


    return 0;
}