#include <iostream>
#include <fstream>


#include "socket.hpp"


using namespace std;

int main(int ac, char **av)
{

    SocketClass s;

    s.run();

    // Request r("GET", string(av[1]), "HTTP/1.1");

    // cout << "bfr " << r.getRessource() << endl;
    // r.resource();
    // cout << "atr " << r.getRessource() << endl;


    return 0;
}