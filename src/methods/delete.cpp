/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-khad <yel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 05:20:49 by yel-khad          #+#    #+#             */
/*   Updated: 2023/04/07 00:32:11 by yel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "delete.hpp"

Delete::Delete(Request request, Server server) : Method(request, server)
{  
    if (!getRequestedResource())
    {
        _status = 404;
        _comment = "Not Found";
        return ;
    }
    if (isFile())
    {
        if (!hasCGI())
        {
            if (!remove(_url.c_str()))
            {
                _status = 403;
                _comment = "Forbidden";
                return ;
            }
            _status = 204;
            _comment = "No Content";
            return ;
        }
        CGI cgi(request, server, _url, "DELETE");
        _resp = cgi.getResp();
        if (_resp == "error")
        {
            _resp = "";
            _status = 502;
            _comment = "Bad Gateway";
        }
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
    string url = _url;
    _url = getIndex();
    if (_url.empty())
    {
        _status = 403;
        _comment = "Forbidden";
        return ;
    }
    if (hasCGI())
    {
        CGI cgi(request, server, _url, "DELETE");
        _resp = cgi.getResp();
        if (_resp == "error")
        {
            _resp = "";
            _status = 502;
            _comment = "Bad Gateway";
            return ;
        }
        _status = 200;
        _comment = "OK";
        return ;
    }
}