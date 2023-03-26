/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-khad <yel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 00:00:05 by yel-khad          #+#    #+#             */
/*   Updated: 2023/03/26 00:45:10 by yel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "get.hpp"

Get::Get(Request request, Server server) : Method(request, server)
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
            _status = 200;
            _comment = "OK";
            _path = _url;
            return ;
        }
        // RUN CGI ON REQUESTED FILE
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
        if(FILE *f = fopen("index.html","r"))
        {
            fclose(f);
            _path = "index.html";
            _status = 200;
            _comment = "OK";
            return ;
        }
        if (!getAutoIndex())
        {
            _status = 403;
            _comment = "Forbidden";
            return ;
        }
        _status = 200;
        _comment = "OK";
        //AUTOINDEX OF THE DIRECTORY
        return;
    }
    string index = getIndex();
    if (index.empty())
    {
        _status = 403;
        _comment = "Forbidden";
        return ;
    }
    if (!hasCGI())
    {
        _status = 200;
        _comment = "OK";
        _path = index;
        return ;
    }
    // RUN CGI ON REQUESTED FILE
}