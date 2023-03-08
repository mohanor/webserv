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

    ConfigParser config(getFileContent("./conf/default.conf"));
    Configuration initail(config.getTokens(), config.getLengthServer());
    initail.getServer();


    return 0;
}