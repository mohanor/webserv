/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 21:57:50 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/03/24 16:47:20 by yoelhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "delete.hpp"

Delete::Delete(Request request, Server server):Method(request, server)
{

    cout << "path => " << _request.getRessource().c_str() << endl;
    getResourceType();
}

Delete::~Delete()
{
    
}

void Delete::MissingBackSlash()
{
    _status = 409;
    _comment = "Conflict";
}

void Delete::deleteAllFolderContent()
{
    struct dirent *dirents;
    DIR *dir = opendir(_request.getRessource().c_str());
    if (dir != NULL)
    {
        dirents = readdir(dir);
        dirents = readdir(dir);

        while ((dirents = readdir(dir)) != NULL)
        {
            string path = _request.getRessource() + dirents->d_name;
          size_t res =  remove(path.c_str());
          if (res == 0)
                deleteSeccess();
        } 
    }
    else
    {
        if (errno == 13)
            setError(403, "Forbidden");
        else
            setError(500, "Internal Server Error");
    }
    
}

void Delete::setError(int status, string comment)
{
    _status = status;
    _comment = comment;
}

void Delete::getResourceType()
{

    if (getRequestedResource())
    {
        if (hasSlashInTheEnd())
            deleteAllFolderContent();
        else
            MissingBackSlash();
         cout << "resource found" << endl;
    }
       
    else if (isFile())
        cout << "file found" << endl;
    else
        cout << "resource not found" << endl;
}


void Delete::deleteResource()
{
    size_t reseltDelete = remove(_request.getRessource().c_str());

    if (reseltDelete == 0)
        deleteSeccess();
    else
        cout << "file not deleted" << endl;
    
}

void Delete::deleteSeccess()
{
    _status = 204;
    _comment = "No Content";
    _path = _request.getRessource();
}