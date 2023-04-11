/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 16:14:00 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/04/09 19:54:13 by yoelhaim         ###   ########.fr       */
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
    string path; //SHOULD HAVE SLASH ONLY IN THE END
    map<string, string> _directives;
    map<string, string> error_page_location;
};


