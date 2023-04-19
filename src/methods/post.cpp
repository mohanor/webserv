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

            if (request.getBody().size() > server.getMaxSize())
            {
                _status = 413;
                _resp = getFileContent(_error_page[_status]);
                _contentType = _mime.getMimeType(_mime.getExtension(getFileContent(_error_page[_status])));
                _comment = "Request Entity Too Large";
                return ;
            }

            if (!request.uploadFile(server.getUploadPath(request.getRessource())))
            {
                _status = 400;
                _resp = getFileContent(_error_page[_status]);
                _contentType = _mime.getMimeType(_mime.getExtension(getFileContent(_error_page[_status])));
                _comment = "Bad Request";
            }
            return ;
        }
        _status = 403;
        _comment = "Forbidden";
        _resp = getFileContent(_error_page[_status]);
        _contentType = _mime.getMimeType(_mime.getExtension(getFileContent(_error_page[_status])));
        return ;
    }
    if (!getRequestedResource())
    {

        _status = 404;
        _comment = "Not Found";
        _resp =getFileContent(_error_page[_status]);
        _contentType = _mime.getMimeType(_mime.getExtension(getFileContent(_error_page[_status])));
        return ;
    }
    if (!isDir())
    {
        if (!hasCGI())
        {
            _status = 403;
            _comment = "Forbidden";
            _resp =getFileContent(_error_page[_status]);
            _contentType = _mime.getMimeType(_mime.getExtension(getFileContent(_error_page[_status])));
            return ;
        }
        CGI cgi(request, server, _url, "POST");
        _resp = cgi.getResp();
        if (_resp == "error")
        {
            _status = 502;
            _resp = getFileContent(_error_page[_status]);
            _contentType = _mime.getMimeType(_mime.getExtension(getFileContent(_error_page[_status])));
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
        _resp = getFileContent(_error_page[_status]);
        _contentType = _mime.getMimeType(_mime.getExtension(getFileContent(_error_page[_status])));
        return ;
    }
    if (!hasIndexFile())
    {
        _status = 403;
        _comment = "Forbidden";
        _resp = getFileContent(_error_page[_status]);
        _contentType = _mime.getMimeType(_mime.getExtension(getFileContent(_error_page[_status])));
        return ;
    }
    _url = getIndex();
    if (_url.empty())
    {
        _status = 404;
        _comment = "Not Found";
        _resp = getFileContent(_error_page[_status]);
        _contentType = _mime.getMimeType(_mime.getExtension(getFileContent(_error_page[_status])));
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

bool Post::locationSupportUpload()
{
    map<string, string> map = _server._locations[_server.getMatchedLocation()]._directives;

    if (map.find("upload_enable") != map.end())
        return (map["upload_enable"] == "on");
    return false;
}