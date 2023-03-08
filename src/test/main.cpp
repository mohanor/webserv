#include <iostream>
#include <fstream>
#include "../../parsing/conf/ConfigParser.hpp"
#include "../utility/utility.hpp"


using namespace std;

int main(int ac, char **av)
{
    // SocketClass s;


    ConfigParser config(getFileContent("./conf/default.conf"));
    // cout << "sddd\n";
    // config.printData();


    return 0;
}