/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-khad <yel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 05:20:49 by yel-khad          #+#    #+#             */
/*   Updated: 2023/04/17 07:11:19 by yel-khad         ###   ########.fr       */
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
                _status = 204;
                _comment = "No Content";
                _resp = getFileContent(_error_page[204]);
                return ;
            }
            _status = 403;
            _resp = getFileContent(_error_page[_status]);
            _comment = "Forbidden";
            return ;
        }
        CGI cgi(request, server, _url, "DELETE");
        _resp = cgi.getResp();
        if (_resp == "error")
        {
            _status = 502;
            _resp = getFileContent(_error_page[_status]);
            _comment = "Bad Gateway";
        }
        deserialize();
        _resp = getRidOfHeaders();
        _status = 204;
        _comment = "No Content";
        return ;
    }
    if (!hasSlashInTheEnd())
    {
        _status = 301;
        _resp = getFileContent(_error_page[_status]);
        _comment = "Moved Permanently";
        return ;
    }
    if (!hasIndexFile())
    {
        _status = 403;
        _resp = getFileContent(_error_page[_status]);
        _comment = "Forbidden";
        return ;
    }
    string url = _url;
    _url = getIndex();
    if (_url.empty())
    {
        _status = 403;
        _resp = getFileContent(_error_page[_status]);
        _comment = "Forbidden";
        return ;
    }
    if (hasCGI())
    {
        CGI cgi(request, server, _url, "DELETE");
        _resp = cgi.getResp();
        if (_resp == "error")
        {
            _status = 502;
            _resp = getFileContent(_error_page[_status]);
            _comment = "Bad Gateway";
            return ;
        }
        deserialize();
        _resp = getRidOfHeaders();
        _status = 200;
        _comment = "OK";
        return ;
    }
}