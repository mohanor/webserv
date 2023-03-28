/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 00:00:05 by yel-khad          #+#    #+#             */
/*   Updated: 2023/03/27 17:54:50 by yoelhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "get.hpp"

Get::Get(Request request, Server server) : Method(request, server)
{
    if (!getRequestedResource())
    {
        _status = 404;
        _comment = "Not Found";
        _path = "./error_page/404.html";
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
        _path = "./error_page/301.html";
        return ;
    }
    if (!hasIndexFile())
    {
        string file = _url + "index.html"; 
        cout << file << endl;
        if(FILE *f = fopen(file.c_str(),"r")) // should be joined with the server root
        {
            fclose(f);
            _path = "./index.html";
            _status = 200;
            _comment = "OK";
            return ;
        }
        if (!getAutoIndex())
        {
            _status = 403;
            _comment = "Forbidden";
            _path = "./error_page/403.html";
            return ;
        }
        Worker::listenDirectory(_url);
        _status = 200;
        _comment = "OK";
        _path = "./configuration/dir/index.html";
        return;
    }
    _url = getIndex();
    if (_url.empty())
    {
        _status = 403;
        _comment = "Forbidden";
        _path = "./error_page/403.html";
        return ;
    }
    if (!hasCGI())
    {
        
        _status = 200;
        _comment = "OK";
        _path = _url;
        return ;
    }
    // RUN CGI ON REQUESTED FILE
}