/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 02:26:41 by matef             #+#    #+#             */
/*   Updated: 2023/02/27 04:48:35 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Request.hpp"

Request::Request(string method, string resource, string version) : _method(method), _resource(resource), _version(version)
{}

Request &Request::operator=(const Request &copy)
{
    if (this != &copy)
    {
        this->_method = copy._method;
        this->_resource = copy._resource;
        this->_version = copy._version;
    }
    return *this;
}

Request::Request(const Request &copy)
{
    *this = copy;
}

vector<string> Request::getVector(string line, char delimiter)
{
    vector<string> v;
    string sub;
    
    stringstream file(line);
    while (getline(file, sub, delimiter))
        v.push_back(sub);
    return v;
}

Request Request::deserialize(const string& request)
{
    vector<string> tokens;
    vector<string> first_line;

    string line;
    stringstream request_as_file(request);

    getline(request_as_file, line);
    first_line = Request::getVector(line);

    if (first_line.size() != 3 || not syntaxIsCorrect(first_line) )
    {
        cerr << "\033[1;31mbold BAD REQUEST" << '\n';
    }
    

    return Request(first_line[0], first_line[1], first_line[2]);
}

string Request::serialize()
{
    return this->_method + " " + this->_resource + " " + this->_version + "\r\n";
}




















// Request Request::deserialize(const std::string& request)
// {
//     string method = "GET";
//     string resource = "/";
//     string version = "HTTP/1.1";



//     return Request(version, method, resource);
// }

Request::~Request() {}
