/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 14:43:45 by matef             #+#    #+#             */
/*   Updated: 2023/04/13 14:54:55 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response()
{
}

Response::Response(const Response &src)
{
    *this = src;
}

Response &Response::operator=(const Response &rhs)
{
    if (this != &rhs)
    {
        _status = rhs._status;
        _headers = rhs._headers;
        _comment = rhs._comment;
    }
    return *this;
}

Response::~Response()
{
}

void Response::setStatus(int status)
{
    _status = status;
}

void Response::setComment(string comment)
{
    _comment = comment;
}

void Response::setHeaders(string key, string value)
{
    _headers[key] = value;
}

string Response::getHeader()
{
    string header;

    header += "HTTP/1.1 " + to_string(_status) + " " + _comment + ENDL;

    for(map<string, string>::iterator it = _headers.begin(); it != _headers.end(); it++)
        header += it->first + ": " + it->second + ENDL;

    header += ENDL;
    return header;
}
