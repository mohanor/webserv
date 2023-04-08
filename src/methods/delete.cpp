/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 21:57:50 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/04/04 01:37:48 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "delete.hpp"

Delete::Delete(Request request, Server server) : Method(request, server)
{    
    getResourceType();
}

Delete::~Delete()
{
}

Delete::Delete(const Delete &copy) : Method(copy)
{
    *this = copy;
}

Delete &Delete ::operator=(const Delete &copy) 
{
    if (this != &copy)
    {  
        _request = copy._request;
        _server = copy._server;
        _url = copy._url;
        _resp = copy._resp;
        _status = copy._status;
        _comment = copy._comment;
    }
    return *this;
}

void Delete::MissingBackSlash()
{
    _status = 409;
    _comment = "Conflict";
}

void Delete::setError(int status, string comment)
{
    _status = status;
    _comment = comment;
}

void Delete::getResourceType()
{
    if (isDir())
    {
        if (hasSlashInTheEnd())
        {
            if (hasCGI())
                cout << "CGI" << endl;
            // deleteAllFolderContent();
        }
        else
            MissingBackSlash();
    }
    else if (isFile())
    {
        if (hasCGI())
            cout << "CGI" << endl;
        else
            deleteResource();
    }
    else
    {
         if (errno == EACCES)
        {
            _resp = getFileContent("./error_page/500.html");
            setError(500, "Internal Server Error");
        }
        else
        {
            _resp = getFileContent("./error_page/404.html");
            setError(404, "Not Found");
        }
    }

}

void Delete::deleteResource()
{
    // size_t reseltDelete = remove(_url.c_str());

    // if (reseltDelete == 0)
    //     deleteSeccess();
    // else
        cout << "file not deleted" << endl;
}

void Delete::deleteSeccess()
{
    _status = 204;
    _comment = "No Content";
}