#include <iostream>
#include <fstream>
#include "../../parsing/conf/ConfigParser.hpp"
#include "../utility/utility.hpp"


using namespace std;

int main(int ac, char **av)
{

    ConfigParser config(getFileContent("./conf/default.conf"));
    // cout << "sddd\n";
    // config.printData();

    // cout << r.serialize();

    return 0;
}