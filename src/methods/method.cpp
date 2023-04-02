#include "method.hpp"

Method::Method(Request request, Server server) : _request(request) , _server(server)
{
    Location loc = _server._locations[_server.getMatchedLocation()];
    string resource = _request.getRessource().erase(0, _server.getMatchedLocation().length());

    if (loc._directives.find("root") != loc._directives.end())
        _url = loc._directives["root"];
    else
        _url = _server.getRoot();
    _url = join_path(_url, resource);
    _status = 405;
    _comment =  "Method Not Allowed";
    _path =  "./error_pages/405.html";
   
}

bool Method::getRequestedResource()
{
    return (isDir() || isFile());
}

bool Method::isFile()
{
    if (isDir())
        return false;
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
    string url = _url;
    string end = _url.erase(0, _url.find_last_of('.'));

    _url =url;
    if (end == ".py" || end == ".php")
        return true;
    return false;
}

string Method::getIndex()
{
    string index = "";
    vector<string> index_v;
    map<string, string> map = _server._locations[_server.getMatchedLocation()]._directives;
    if(map.find("index") != map.end())
    {
        index_v = Request::getVector(map["index"]);
        for (int i=0; i < index_v.size(); i++)
        {
            if (FILE *f = fopen(join_path(_url,index_v[i]).c_str(), "r"))
                index = join_path(_url,index_v[i]);
        }
    }
    else
    {
        index_v = Request::getVector(_server.getIndex());
        for (int i=0; i < index_v.size(); i++)
        {
            if (FILE *f = fopen(join_path(_url,index_v[i]).c_str(), "r"))
                index = join_path(_url,index_v[i]);
        }
    }
    
    return index;
}

string join_path(string s1, string s2)
{
    if (s1.back() == '/' && s2[0] == '/')
        return (s1.erase(s1.length() - 1, s1.length() - 1) + s2);
    if (s1.back() == '/' || s2[0] == '/')
        return (s1 + s2);
    return (s1 + "/" + s2);
}


int Method::getStatus() const
{
    return _status;
}

string Method::getComment() const
{
    return _comment;
}

string Method::getURL() const
{
    return _url;
}

string Method::getPath() const
{
    return _path;
}