#include <iostream>
#include <fstream>
#include "../../parsing/conf/ConfigParser.hpp"
#include "../../configuration/Configuration.hpp"
#include "../utility/utility.hpp"
#include "socket.hpp"


using namespace std;

int main(int ac, char **av)
{
    // SocketClass s;
    // s.run();

    Configuration config("./conf/default.conf");

    vector<Server> _server = config.getServers();

    cout << _server.size() << endl;

    cout << _server[0].getLengthLocation() << endl;
  
    return 0;
}