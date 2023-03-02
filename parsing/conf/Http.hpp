/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 00:23:47 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/03/02 14:26:00 by yoelhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
using namespace std;

class Http
{
protected:
    size_t _cli_max_size;
    string _root;
    string _index;
    string _error_page;
    string _allowed_methods;
    bool _autoindex;

public:
    Http(size_t _cli_max_size = 1024, string _root = "/www/html", string _index = "index.html", string _error_page = "404.html", string _allowed_methods = "GET", bool _autoindex = true);
    Http(const Http &copy);
    Http &operator=(const Http &copy);
    ~Http();

    size_t getMaxSize() const;
    string getRoot() const;
    string getIndex() const;
    string getErrorPage(int index) const;
    string getAllowedMethods(int index) const;
    bool getAutoIndex() const;

    void setMaxSize(size_t size);
    void addAllowedMethods(string method);
    void setRoot(string root);
    void setIndex(string index);
    void setErrorPage(string path);
    void setAutoIndex(bool value);
};