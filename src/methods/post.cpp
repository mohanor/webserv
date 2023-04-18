#include "post.hpp"
#include "method.hpp"

Post::Post(Request request, Server server) : Method(request, server)
{

    if (request.isUploadable())
    {
        if (locationSupportUpload())
        {
            _status = 201;
            _resp = getFileContent(_error_page[_status]);
            _comment = "Created";
            request.uploadFile(server.getUploadPath(request.getRessource()));
            return ;
        }
        _status = 403;
        _comment = "Forbidden";
        return ;
    }
    if (!getRequestedResource())
    {

        _status = 404;
        _comment = "Not Found";
        return ;
    }
    if (!isDir())
    {
        if (!hasCGI())
        {
            _status = 403;
            _comment = "Forbidden";
            return ;
        }
        CGI cgi(request, server, _url, "POST");
        _resp = cgi.getResp();
        if (_resp == "error")
        {
            _resp = "";
            _status = 502;
            _comment = "Bad Gateway";
            return ;
        }
        deserialize();
        _resp = getRidOfHeaders();
        _status = 200;
        _comment = "OK";
        return ;
    }
    if (!hasSlashInTheEnd())
    {
        _status = 301;
        _comment = "Moved Permanently";
        return ;
    }
    if (!hasIndexFile())
    {
        _status = 403;
        _comment = "Forbidden";
        return ;
    }
    _url = getIndex();
    if (_url.empty())
    {
        _status = 403;
        _comment = "Forbidden";
        return ;
    }
    CGI cgi(request, server, _url, "POST");
    _resp = cgi.getResp();
    if (_resp == "error")
    {
        _resp = "";
        _status = 502;
        _comment = "Bad Gateway";
        return ;
    }
    deserialize();
//     for(map<string,string >::const_iterator it = _headers.begin();
//     it != _headers.end(); ++it)
// {
//     std::cout << it->first << " " << it->first << " " << it->second << "\n";
// }
    _resp = getRidOfHeaders();
    _status = 200;
    _comment = "OK";
    return ;
}

bool Post::locationSupportUpload()
{
    map<string, string> map = _server._locations[_server.getMatchedLocation()]._directives;

    if (map.find("upload_enable") != map.end())
        return (map["upload_enable"] == "on");
    return false;
}