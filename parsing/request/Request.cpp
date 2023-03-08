/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 02:26:41 by matef             #+#    #+#             */
/*   Updated: 2023/03/08 16:46:28 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Request.hpp"

Request::Request(string method, string resource, string version, map<string, Header> headers)
{
	this->_method = method, this->_resource = resource;
	this->_version = version, this->_headers = headers;
}

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
	{
		if (sub != "")
        	v.push_back(sub);
	}
    return v;
}

bool Request::validVersion()
{
	return this->_version == "HTTP/1.1";
}

Request Request::deserialize(const string& request)
{
	map<string, Header> headers;
	map<string, Header>::iterator it;
	
	vector<string> 		tokens;
	vector<string> 		headerFirstLine;

	string line;
	stringstream requestAsFile(request);

	getline(requestAsFile, line);
	headerFirstLine = Request::getVector(line);

	if (headerFirstLine.size() != 3)
		goto error;

	while (getline(requestAsFile, line))
	{
		tokens = getVector(line);
		
		string key = tokens[0];
		string value = line.substr(key.length());
		
		headers.insert(make_pair(key, Header(key, value)));
	}

	return Request(headerFirstLine[0], headerFirstLine[1], headerFirstLine[2], headers);

	error:
		cerr << request << "\n" << request.size() << " " << request.length() << '\n';
		return Request("", "", "");
}

bool Request::syntaxError()
{
	if (! hasOnlyUppercase(_method) || ! hasOnlyUppercase(_version) || validVersion())
		return true;
	return false;
}

void Request::resource()
{
	deque<string> s;

	vector<string> v = getVector(_resource, '/');
	vector<string>::iterator it = v.begin();

	while (it != v.end())
	{
		if (*it == ".." && !s.empty())
			s.pop_back();
		else if (*it != "." && *it != "..")
			s.push_back(*it);
		it++;
	}

	if (s.empty())
		_resource = "/";
	else
	{
		_resource = "";
		while (!s.empty())
		{
			_resource += "/" + s.front();
			s.pop_front();
		}
	}
	
}

string Request::getRessource()
{
	return _resource;
}

string Request::serialize()
{
	string endLine = "\r\n";
	string request;
	
    request = this->_method + " " + this->_resource + " " + this->_version + endLine;
	
	Headers::iterator it = _headers.begin();
	while (it != _headers.end())
	{
		request += it->second.getKey() + it->second.getValue() + endLine;	
		it++;
	}

	return request;
}





















// Request Request::deserialize(const std::string& request)
// {
//     string method = "GET";
//     string resource = "/";
//     string version = "HTTP/1.1";



//     return Request(version, method, resource);
// }

Request::~Request() {}
