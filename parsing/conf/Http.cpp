/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 00:50:32 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/04/17 22:58:54 by yoelhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"
Http::Http(directives dir) 
{
    this->_cli_max_size = dir.cli_max_size;
    this->_root = dir.root;
    this->_index = dir.index;
    this->_error_page = dir.error_page;
    this->_allowed_methods = dir.allow;
    this->_autoindex = dir.autoindex;
}

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

map<int, string > Http::getErrorPage() const
{
    return this->_error_page;
}

string Http::getAllowedMethods(int) const
{
    return this->_allowed_methods;
}

bool Http::getAutoIndex() const
{
    return this->_autoindex;
}

