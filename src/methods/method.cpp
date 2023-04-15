/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 15:13:18 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/04/14 18:40:28 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "method.hpp"

Method::Method(Request request, Server server) : _request(request) , _server(server) ,_resp(""), _contentType("text/html")
{
    Location loc = _server._locations[_server.getMatchedLocation()];
    // cout << "fff " << _server.getMatchedLocation() << endl;
    string resource = _request.getRessource().erase(0, _server.getMatchedLocation().length());

    
    if (loc._directives.find("root") != loc._directives.end())
        _url = loc._directives["root"];
    else
        _url = _server.getRoot();
    
    _url = join_path(_url, resource);
    // cout << "url " << _url << endl;
    
    insetErrorPage();
    
    _status = 404;
    _comment =  " Not Found";
    _resp =  getFileContent(_error_page[404]);
}

void Method::insetErrorPage()
{
    string statusCode = "201 301 400 401 403 404 405 408 413 414 500 501 502 503 504 505";

    vector<string> status = Request::getVector(statusCode);

    for (size_t i = 0; i < status.size(); i++)
         this->_error_page[atoi(status[i].c_str())] = "./error_page/" + status[i] + ".html";
    
         map<string, string>::iterator maps = _server._locations[_server.getMatchedLocation()].error_page_location.begin();

         while (maps != _server._locations[_server.getMatchedLocation()].error_page_location.end())
         {
            this->_error_page[atoi(maps->first.c_str())] = maps->second;
            maps++;
         }
    
        vector<pair<int, string> > err = _server.getErrorPage();
        for (size_t i = 0; i < err.size(); i++)
        {
            if (_error_page.find(err[i].first) != _error_page.end())
                this->_error_page[err[i].first] = err[i].second;
        }
}

Method::Method(int status, string comment, string url, Request request, Server server) : _request(request),_server(server),  _status(status), _comment(comment),  _url(url) 
{}



bool Method::getRequestedResource()
{
    return (isDir() || isFile());
}

bool Method::isFile()
{
    if (isDir())
        return false;
    FILE *fp = fopen(_url.c_str(), "r");
    if (fp)
    {
        fclose(fp);
        return true;
    }
    return false;
}

bool Method::isDir()
{
    DIR *dir = opendir(_url.c_str());
    if (dir)
    {
        closedir(dir);
        return true;
    }
    return false;
}

bool Method::hasSlashInTheEnd()
{
    return (_url.back() == '/');
}

bool Method::hasIndexFile()
{
    map<string, string> map = _server._locations[_server.getMatchedLocation()]._directives;
    return (map.find("index") != map.end() || !_server.getIndex().empty());
}

bool Method::getAutoIndex()
{
    map<string, string> map = _server._locations[_server.getMatchedLocation()]._directives;

    if (map.find("autoindex") != map.end())
        return (map["autoindex"] == "on");
    return (_server.getAutoIndex());
}

bool Method::hasCGI()
{
    string url = _url;
    string end = _url.erase(0, _url.find_last_of('.'));

    _url =url;
    if (end == ".py" || end == ".php")
        return true;
    return false;
}

string Method::getIndex()
{
    string index = "";
    vector<string> index_v;
    map<string, string> map = _server._locations[_server.getMatchedLocation()]._directives;
    if(map.find("index") != map.end())
    {
        index_v = Request::getVector(map["index"]);
        for (size_t i=0; i < index_v.size(); i++)
        {
            if (FILE *f = fopen(join_path(_url,index_v[i]).c_str(), "r"))
            {
                index = join_path(_url,index_v[i]);
                fclose(f); //? here
            }
        }
    }
    else
    {
        index_v = Request::getVector(_server.getIndex());
        for (size_t i=0; i < index_v.size(); i++)
        {
            if (FILE *f = fopen(join_path(_url,index_v[i]).c_str(), "r"))
            {
                index = join_path(_url,index_v[i]);
                fclose(f); //? here
            }
        }
    }
    return index;
}

string Method::getContentType()
{  
    return _contentType;
}

string join_path(string s1, string s2)
{
    if (s1.back() == '/' && s2[0] == '/')
        return (s1.erase(s1.length() - 1, s1.length() - 1) + s2);
    if (s1.back() == '/' || s2[0] == '/')
        return (s1 + s2);
    return (s1 + "/" + s2);
}


int Method::getStatus() const
{
    return _status;
}

string Method::getComment() const
{
    return _comment;
}

string Method::getURL() const
{
    return _url;
}


string Method::getResponse() const
{
    return this->_resp;
}



