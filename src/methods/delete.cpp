/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 21:57:50 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/03/25 20:25:52 by yoelhaim         ###   ########.fr       */
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

Delete &Delete::operator=(const Delete &copy) 
{
    if (this != &copy)
    {  
        _request = copy._request;
        _server = copy._server;
        _url = copy._url;
        _path = copy._path;
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

void Delete::deleteAllFolderContent()
{
    struct dirent *dirents;
    DIR *dir = opendir(_url.c_str());
    int countFiles = 0;

    
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
            else
                cout << "file not deleted not found" << endl;
            countFiles++;
        }
         if (countFiles == 0)
         {
            _path = "./error_page/404.html";
            setError(404, "Not Found");
         }
        
    }
    else
        return;
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
    {
         if (errno == EACCES)
        {
            _path = "./error_page/500.html";
            setError(500, "Internal Server Error");
        }
        else
        {
            _path = "./error_page/404.html";
            setError(404, "Not Found");
        }
    }

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
}