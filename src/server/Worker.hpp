/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Worker.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 22:36:44 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/03/23 01:47:46 by yoelhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string.h>
#include <map>
#include <vector>

#include "../../parsing/conf/Server.hpp"
#include "../../parsing/conf/Location.hpp"
#include  "../../parsing/request/Request.hpp"



class Worker
{
private:
    /* data */
public:
    Worker();
    ~Worker();
    Worker(const Worker &copy);
    Worker &operator=(const Worker &copy);
    bool getMatchedLocationFoRequestUri(string requestUri,  Server &servers);
    bool isLocationHaveRedirection(Server &servers);
    bool isMethodAllowdedInLocation(string Method,Server &servers);
    bool getAllowdedMethods(string methods, string allow);
};

