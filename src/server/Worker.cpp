/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Worker.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 22:37:31 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/04/17 19:49:29 by yoelhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Worker.hpp"
#include "../methods/get.hpp"

Worker::Worker()
{
}

Worker::~Worker()
{
}

Worker::Worker(const Worker &copy)
{
    *this = copy;
}

Worker &Worker::operator=(const Worker &copy)
{
    if (this != &copy)
    {
        /* data */
    }
    return *this;
}

bool Worker::getMatchedLocationFoRequestUri(string requestUri, Server &servers)
{
    string location;
    size_t sizeLocation;

    map<string, Location>::iterator it = servers._locations.begin();
    vector<string> uri = Request::getVector(requestUri, '/');

    sizeLocation = uri.size();
    
    if (sizeLocation <= 1)
        return (servers.setMatchedLocation("/"), true);

    while (sizeLocation)
    {

        for (size_t i = 0; i < uri.size(); i++)
            location += "/" + uri[i];

        while (it != servers._locations.end())
        {
            if (it->first == location)
                return (servers.setMatchedLocation(it->first), true);
            it++;
        }
        it = servers._locations.begin();
        uri.pop_back();
        location.clear();
        sizeLocation--;
    }
    return (false);
}

bool Worker::isLocationHaveRedirection(Server &servers, string &path)
{
    map<string, string> maps = servers._locations[servers.getMatchedLocation()]._directives;
    // path = maps["return"];
    return maps.find("return") != maps.end() ? path = maps["return"], true : false;
}

bool Worker::getAllowdedMethods(string methods, string allow)
{
    vector<string> methodsVector = Request::getVector(methods);
    for (size_t i = 0; i < methodsVector.size(); i++)
        if (allow == methodsVector[i])
            return true;
    return false;
}

bool Worker::isMethodAllowdedInLocation(string Method, Server &servers)
{
    map<string, string> maps = servers._locations[servers.getMatchedLocation()]._directives;

    if (maps.find("allow") != maps.end())
        return getAllowdedMethods(maps["allow"], Method);
    else
        return getAllowdedMethods(servers.getAllowedMethods(1), Method);

    return false;
}

/************************************************************
 *                METHODS GET DELETE POST                   *
 *************************************************************/

Delete Worker::runMethodDelete(Request &req, Server &server)
{
    Delete del(req, server);

    return del;
}

Get Worker::runMethodGet(Request &req, Server &server)
{
    Get get(req, server);
    return get;
}

Post Worker::runMethodPost(Request &req, Server &server)
{
    Post post(req, server);
    return post;
}

bool Worker::checkLocations(Request &req, Server &server, bool &isRedirection, string &path, bool &method)
{
    if (getMatchedLocationFoRequestUri(req.getRessource(), server))
    {
        if (isLocationHaveRedirection(server, path))
            return (isRedirection = true, false);

        else if (!isMethodAllowdedInLocation(req.getMethod(), server))
            return (method = true, false);

        return true;
    }

    return false;
}

Method Worker::getMethodObject(Request req, Server server)
{
    bool isRedirection = false;
    bool methods = false;
    string path;

    if (checkLocations(req, server, isRedirection, path, methods))
    {
        if (req.getMethod() == "DELETE")
            return runMethodDelete(req, server);
        else if (req.getMethod() == "GET")
            return runMethodGet(req, server);
        else
            return runMethodPost(req, server);
    }
    else if (methods)
        return Method(405, "Method Not Allowed ", "", req, server);
    else if (isRedirection)
    {
        vector<string> urlVector = Request::getVector(path);
        string messageRedirect;
        switch (atoi(urlVector[0].c_str()))
        {
        case 301:
            messageRedirect = "Moved Permanently";
            break;
        case 302:
            messageRedirect = "Found";
            break;
        case 303:
            messageRedirect = "See Other";
            break;
        case 307:
            messageRedirect = "Temporary Redirect";
            break;
        default:
            break;
        }

        return Method(atoi(urlVector[0].c_str()), messageRedirect, urlVector[1], req, server);
    }
    return Method(req, server);
}

/************************************************************
 *                         LISTEN DIRE                      *
 *************************************************************/

string Worker::listenDirectory(string pathUri, string pathDir)
{
    dirent *pdirent;
    DIR *pdir;
    pdir = opendir(pathUri.c_str());

    string path;
    fstream file("./error_page/index.html");

    string str;
    while (getline(file, str, '\0'))
        ;

    if (pdir == NULL)
        return "Error opening directory";

    pdirent = readdir(pdir);
    while ((pdirent = readdir(pdir)))
    {
        std::string file = "<i class='fa fa-file' style='padding:3px;color:gray'></i>";
        std::string folder = "<i class='fa fa-folder' style='padding:3px;color:#F5E588'></i>";
        std::string typePath = pdirent->d_type != 4 ? file : folder;

        std::string htmlContent = "<tr><td>" + typePath + "<a href='" + pathDir + "/" + pdirent->d_name + "'>" + pdirent->d_name + "</a></td></tr>\n";
        path.append(htmlContent);
    }

    std::string to_find = "html_content";

    size_t findindex = str.find(to_find.c_str());

    if (findindex != std::string::npos)
    {
        str.replace(findindex, to_find.size(), path);
        findindex += path.length();
    }

    size_t pathIndex = str.find("PATHNAME");
    if (pathIndex != std::string::npos)
    {
        str.replace(pathIndex, 8, pathUri);
        pathIndex += pathUri.length();
    }
    std::ofstream out("./configuration/dir/index.html");
    out << str;
    closedir(pdir);
    file.close();
    out.close();

    return str;
}