#include "method.hpp"

Method::Method(Request request, Server server) : _request(request) , _server(server)
{
    Location loc = _server._locations[_server.getMatchedLocation()];
    string resource = _request.getRessource().erase(0, _server.getMatchedLocation().length());

    if (loc._directives.find("root") != loc._directives.end())
        _url = loc._directives["root"];
    else
        _url = _server.getRoot();
    if (_url.back() == '/' || resource[0] == '/')
        _url += resource;
    else 
        _url += "/" + resource;
}

bool Method::getRequestedResource()
{
    return (isDir() || isFile());
}

bool Method::isFile()
{
    FILE *fp = fopen(_url.c_str(), "r");
    if (fp)
    {
        fclose(fp);
        return true;
    }
    return false;
}

bool Method::isDir()
{
    DIR *dir = opendir(_url.c_str());
    if (dir)
    {
        closedir(dir);
        return true;
    }
    return false;
}

bool Method::hasSlashInTheEnd()
{
    return (_url.back() == '/');
}

bool Method::hasIndexFile()
{
    map<string, string> map = _server._locations[_server.getMatchedLocation()]._directives;
    return (map.find("index") != map.end() || !_server.getIndex().empty());
}

bool Method::getAutoIndex()
{
    map<string, string> map = _server._locations[_server.getMatchedLocation()]._directives;

    if (map.find("autoindex") != map.end())
        return (map["autoindex"] == "on");
    return (_server.getAutoIndex());
}

bool Method::hasCGI()
{
    string end = _url.erase(0, _url.find_last_of('.'));

    if (end == ".py" || end == ".php")
        return true;
    return false;
}

