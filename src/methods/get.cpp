/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-khad <yel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 00:00:05 by yel-khad          #+#    #+#             */
/*   Updated: 2023/03/25 02:37:24 by yel-khad         ###   ########.fr       */
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
            if (FILE *f = fopen(index_v[i].c_str(), "r"))
                index = join_path(_url,index_v[i]);
        }
    }
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