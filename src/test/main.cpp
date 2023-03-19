#include <iostream>
#include <fstream>
#include "../../parsing/conf/ConfigParser.hpp"
#include "../../configuration/Configuration.hpp"
#include "../utility/utility.hpp"

#include "../../parsing/mime/MimeTypes.hpp"

#include "socket.hpp"


using namespace std;

int main(int ac, char **av)
{
    // SocketClass s;
    // s.run();

    string file = "./conf/mime.types";

    MimeTypes m;
    m.parseMimeTypes(file);

    cout << m.getMimeType("htm") << endl;

    

    return 0;
}