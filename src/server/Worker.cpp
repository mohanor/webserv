/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Worker.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 22:37:31 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/03/25 02:38:25 by yoelhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Worker.hpp"

Worker::Worker(/* args */)
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
    map<string, Location>::iterator it = servers._locations.begin();

    vector<string> uri = Request::getVector(requestUri, '/');

   
        size_t sizeLocation = uri.size() ;
        string location = "";

        while (sizeLocation)
        {
           
            
            for (size_t i = 0; i < uri.size(); i++)
                location += "/" + uri[i];
                
           cout << "locations: " << location << endl;
            while (it != servers._locations.end())
            {
                cout << "location: " << it->first << endl;
                if (it->first == location)
                    return (servers.setMatchedLocation(it->first) ,true);
                it++;
            }
            it = servers._locations.begin();
            uri.pop_back();
            location.clear();
            sizeLocation--;
        }
        
    return (false);
}

bool Worker::isLocationHaveRedirection(Server &servers)
{
    map<string, string> maps = servers._locations[servers.getMatchedLocation()]._directives;
    
    return maps.find("return") != maps.end();
}

bool Worker::getAllowdedMethods(string methods, string allow)
{
    vector<string> methodsVector = Request::getVector(methods);
     for (size_t i = 0; i < methodsVector.size(); i++)
            if (allow == methodsVector[i])
                return true;
    return false;
}


bool Worker::isMethodAllowdedInLocation(string Method,Server &servers)
{
    map<string, string> maps = servers._locations[servers.getMatchedLocation()]._directives;

    if (maps.find("allow") != maps.end())
        return getAllowdedMethods(maps["allow"], Method);
    else
        return getAllowdedMethods(servers.getAllowedMethods(1), Method);
    
    return false;
}
