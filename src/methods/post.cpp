#include "post.hpp"
#include "method.hpp"

Post::Post(Request request, Server server) : Method(request, server)
{
    if (locationSupportUpload())
    {
        _status = 201;
        _comment = "Created";
        // UPLOAD FILE
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
        // RUN CGI ON REQUESTED FILE
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
    string index = getIndex();
    if (index.empty())
    {
        _status = 403;
        _comment = "Forbidden";
        return ;
    }

    // RUN CGI ON INDEX
}

    bool Post::locationSupportUpload()
    {
        return false;
    }