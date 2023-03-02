/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 00:50:32 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/03/02 14:26:36 by yoelhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"

Http::Http(size_t _cli_max_size, string _root, string _index, string _error_page, string _allowed_methods, bool _autoindex) : _cli_max_size(_cli_max_size), _root(_root), _index(_index), _error_page(_error_page), _allowed_methods(_allowed_methods), _autoindex(_autoindex) {}

Http::~Http() {}

Http::Http(const Http &copy)
{
    *this = copy;
}
Http &Http::operator=(const Http &copy)
{
    if (this != &copy)
    {
        this->_cli_max_size = copy._cli_max_size;
        this->_root = copy._root;
        this->_index = copy._index;
        this->_error_page = copy._error_page;
        this->_allowed_methods = copy._allowed_methods;
        this->_autoindex = copy._autoindex;
    }
    return *this;
}

size_t Http::getMaxSize() const
{
    return this->_cli_max_size;
}

string Http::getRoot() const
{
    return this->_root;
}

string Http::getIndex() const
{
    return this->_index;
}

string Http::getErrorPage(int index) const
{
    return this->_error_page;
}

string Http::getAllowedMethods(int index) const
{
    return this->_allowed_methods;
}

bool Http::getAutoIndex() const
{
    return this->_autoindex;
}

void Http::addAllowedMethods(string method)
{
    _allowed_methods = method;
}


void Http::setMaxSize(size_t size)
{
    _cli_max_size = size;
}

void Http::setRoot(string root)
{
    _root = root;
}
void Http::setIndex(string index)
{
    _index = index;
}
void Http::setErrorPage(string path)
{
    _error_page = path;
}
void Http::setAutoIndex(bool value)
{
    _autoindex = value;
}
