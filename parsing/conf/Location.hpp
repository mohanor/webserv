/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 16:14:00 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/03/09 00:22:24 by yoelhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <map>

// #include "Server.hpp"
#include "Http.hpp"

using namespace std;



struct Location 
{
    
    string path;
    string _return;
    map<string, string> _directives;

// public:
//     Location();
//     Location(directives dir);
//     ~Location();
//     string getReturn() const;
};
