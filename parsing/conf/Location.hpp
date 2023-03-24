/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-khad <yel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 16:14:00 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/03/24 06:19:35 by yel-khad         ###   ########.fr       */
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
};


