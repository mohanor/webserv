#include <iostream>
#include <fstream>
#include "../main/webserv.hpp"
#include "../utility/utility.hpp"

#define PORT 80

using namespace std;

int main(int ac, char **av)
{

    std::string config_file = "./conf/default.conf";

    if (ac == 2)
        config_file = av[1];
    else if (ac > 2)
    {
        std::cout << "Usage: ./webserv <config_file>" << std::endl;
        return 1;
    }

    std::cout << "Config file: --> " << config_file << " <--" << std::endl;

    std::cout << get_file_content(config_file);



    return 0;
}