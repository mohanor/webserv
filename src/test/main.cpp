#include <iostream>
#include <fstream>
#include "../../parsing/conf/ConfigParser.hpp"
#include "../../configuration/Configuration.hpp"
#include "../utility/utility.hpp"

#include "../../parsing/mime/MimeTypes.hpp"

#include "socket.hpp"
#include "Client.hpp"

#include "../../parsing/request/Request.hpp"

using namespace std;

void test(ifstream *file)
{
    string line;
    getline(*file, line);
    cout << line << endl;
    
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

    // cout << c.getPacket() << endl;
    SocketClass s;
    s.run();

    return 0;
}