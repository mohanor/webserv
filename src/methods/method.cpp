#include "method.hpp"

Method::Method(Request request, Server server) : _request(request) , _server(server)
{
    return ;
}

bool Method::getRequestedResource()
{
    DIR *dir = opendir(_request.getRessource().c_str());
    if (dir)
    {
        closedir(dir);
        return true;
    }
    return isFile();
}

bool Method::isFile()
{
    FILE *fp = fopen(_request.getRessource().c_str(), "r");
    if (fp)
    {
        fclose(fp);
        return true;
    }
    return false;
}

bool Method::hasSlashInTheEnd()
{
    return (_request.getRessource().back() == '/');
}

bool Method::hasIndexFile()
{
    map<string, string> map = _server._locations[_request.getRessource()]._directives;
    return (map.find("index") != map.end());
}

bool Method::getAutoIndex()
{
    
    return false;
}

bool Method::hasCGI()
{
    return false;
}

