/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Worker.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 22:36:44 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/04/09 01:24:17 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string.h>
#include <map>
#include <vector>
#include <dirent.h>
#include <fstream>

#include "../../parsing/conf/Server.hpp"
#include "../../parsing/conf/Location.hpp"
#include  "../../parsing/request/Request.hpp"
// #include "../methods/method.hpp"
#include "../methods/delete.hpp"
#include "../methods/post.hpp"
class Get;
// class Post;


class Worker
{
private:
    
public:
    Worker();
    ~Worker();
    Worker(const Worker &copy);
    Worker &operator=(const Worker &copy);
    bool getMatchedLocationFoRequestUri(string requestUri,  Server &servers);
    bool isLocationHaveRedirection(Server &servers,string &path);
    bool isMethodAllowdedInLocation(string Method,Server &servers);
    bool getAllowdedMethods(string methods, string allow);
    Method getMethodObject(Request req, Server server);
    Get  runMethodGet(Request &req, Server &server);
    //void  runMethodPost(Request &req, Server &server);
    Delete  runMethodDelete(Request &req, Server &server);
    static string listenDirectory(string path);
    bool checkLocations(Request &req, Server &server,bool &isRedirection, string &path);
    Post runMethodPost(Request &req, Server &server);
    
};

