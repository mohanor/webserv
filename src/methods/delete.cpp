/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 21:57:50 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/03/25 02:26:06 by yoelhaim         ###   ########.fr       */
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

void Delete::MissingBackSlash()
{
    _status = 409;
    _comment = "Conflict";
}

void Delete::deleteAllFolderContent()
{

    struct dirent *dirents;
    DIR *dir = opendir(_url.c_str());
    if (dir != NULL)
    {
        dirents = readdir(dir);
        dirents = readdir(dir);

        while ((dirents = readdir(dir)) != NULL)
        {
            string path = _url + dirents->d_name;
            size_t res = remove(path.c_str());
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
    if (isDir())
    {
        if (hasSlashInTheEnd())
            deleteAllFolderContent();
        else
            MissingBackSlash();
    }
    else if (isFile())
        deleteResource();
    else
        cout << "file not found" << endl;

}

void Delete::deleteResource()
{
    size_t reseltDelete = remove(_url.c_str());

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