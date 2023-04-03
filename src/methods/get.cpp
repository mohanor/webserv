/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-khad <yel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 00:00:05 by yel-khad          #+#    #+#             */
/*   Updated: 2023/04/03 06:25:04 by yel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "get.hpp"

Get::Get(Request request, Server server) : Method(request, server)
{
    if (!getRequestedResource())
    {
        _status = 404;
        _comment = "Not Found";
        _resp = getFileContent("./error_page/404.html");
        return ;
    }
    if (isFile())
    {
        if (!hasCGI())
        {
            _status = 200;
            _comment = "OK";
            _resp = getFileContent(_url);
            return ;
        }
        CGI cgi(request, server, _url, "GET");
        cout << cgi.getResp() << endl; 
        return ;
    }
    if (!hasSlashInTheEnd())
    {
        _status = 301;
        _comment = "Moved Permanently";
        _resp = getFileContent("./error_page/301.html");
        return ;
    }
    if (!hasIndexFile())
    {
        string file = _url + "index.html"; 
        cout << file << endl;
        if(FILE *f = fopen(file.c_str(),"r")) // should be joined with the server root
        {
            fclose(f);
            _resp = getFileContent("./index.html");
            _status = 200;
            _comment = "OK";
            return ;
        }
        if (!getAutoIndex())
        {
            _status = 403;
            _comment = "Forbidden";
            _resp = getFileContent("./error_page/403.html");
            return ;
        }
        Worker::listenDirectory(_url);
        _status = 200;
        _comment = "OK";
        _resp = getFileContent("./configuration/dir/index.html");
        return;
    }
    _url = getIndex();
    if (_url.empty())
    {
        _status = 403;
        _comment = "Forbidden";
        _resp = getFileContent("./error_page/403.html");
        return ;
    }
    if (!hasCGI())
    {
        
        _status = 200;
        _comment = "OK";
        _resp = getFileContent(_url);
        return ;
    }
    // RUN CGI ON REQUESTED FILE
}