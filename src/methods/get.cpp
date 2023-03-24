/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-khad <yel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 00:00:05 by yel-khad          #+#    #+#             */
/*   Updated: 2023/03/24 06:44:23 by yel-khad         ###   ########.fr       */
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
            _path = _request.getRessource();
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
        if (!getAutoIndex())
        {
            // Possibility of looking for "index.html"
            _status = 403;
            _comment = "Forbidden";
            return ;
        }
        _status = 200;
        _comment = "OK";
        //AUTOINDEX OF THE DIRECTORY
        return;
    }
    _url += _server.getIndex(); // CHECK IF THS FILE EXISTS
    if (!hasCGI())
    {
        _status = 200;
        _comment = "OK";
        _path = _request.getRessource();
        return ;
    }
    // RUN CGI ON REQUESTED FILE
}