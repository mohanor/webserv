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

    


MimeTypes _mime;

void handleSigPipe()
{
    signal(SIGPIPE, SIG_IGN);
}

int main(int ac, char **av)
{

    string config_file = "./conf/default.conf";

    if (ac > 2)
    {
        cout << "Usage: ./webserv [config_file] (config_file is optional)" << endl;
        return 1;
    }

    if (ac == 2)
    {
        config_file = av[1];
        ifstream file(config_file);
        if (!file.is_open())
        {
            cout << "Error: config file does not exist" << endl;
            return 1;
        }
        file.close();
    }

    handleSigPipe();

    SocketClass s(config_file);
    s.run();

    return 0;
}