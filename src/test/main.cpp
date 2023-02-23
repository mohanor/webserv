#include <iostream>
#include "../main/webserv.hpp"

#define PORT 80

int main(int ac, char **av)
{

    std::string config_file = "./config/webserv.conf";

    if (ac == 2)
        config_file = av[1];
    else if (ac > 2)
    {
        std::cout << "Usage: ./webserv <config_file>" << std::endl;
        return 1;
    }

    std::cout << "Config file: --> " << config_file << " <--" << std::endl;


    return 0;
}