/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 02:26:41 by matef             #+#    #+#             */
/*   Updated: 2023/03/24 20:10:06 by matef            ###   ########.fr       */
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

string Request::getMethod()
{
	return _method;
}

string Request::getVersion()
{
	return _version;
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


bool Request::allowedChars()
{
	string allowd_Chars = ALLOWED_CHARACTERS;

	for (size_t i = 0; i < _resource.length(); i++)
	{
		if (allowd_Chars.find(_resource[i]) == std::string::npos)
			return false;
	}
	
	return true;
}

bool Request::isMethodAllowed()
{
	return (_method != "GET" && _method != "DELETE" && _method != "POST");
}

// bool Request::transferEncoding()
// {
// 	return (_headers.find("Transfer-Encoding") != _headers.end()\
// 			&& _headers["Transfer-Encoding"].getValue() != "chunked");
// }

bool Request::acceptUriLength()
{
	return (_resource.length() > URI_MAX_SIZE);
}

bool Request::isVersionSupported()
{
	return (_version != "HTTP/1.1");
}

int Request::isReqWellFormed()
{
	// if (isMethodAllowed()) return METHOD_NOT_ALLOWED;
	
	// if ( transferEncoding() ) 	return NOT_IMPLEMENTED;
	if ( !allowedChars() ) 		return BAD_REQUEST;
	if ( acceptUriLength() ) 	return REQUEST_URI_TOO_LONG;
	if ( isVersionSupported() ) return VERSION_NOT_SUPPORTED;

	// MAX_BODY_SIZE
	return OK;
}




















// Request Request::deserialize(const std::string& request)
// {
//     string method = "GET";
//     string resource = "/";
//     string version = "HTTP/1.1";



//     return Request(version, method, resource);
// }

Request::~Request() {}
