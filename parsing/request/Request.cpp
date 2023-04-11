/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 02:26:41 by matef             #+#    #+#             */
/*   Updated: 2023/04/09 02:51:06 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Request.hpp"


Request::Request(string method, string resource, string version, map<string, string> headers) : _isUploadable(false)
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
		this->_headers = copy._headers;
		this->_body = copy._body;
		this->_queryString = copy._queryString;
		this->_isUploadable = copy._isUploadable;
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

Request Request::deserialize(string request)
{
	string head;
	string body;
	
	map<string, string> headers;
	map<string, Header>::iterator it;
	
	vector<string> 		tokens;
	vector<string> 		headerFirstLine;

	string line;

	
	stringstream ss(request);
	
	while (getline(ss, line))
	{
		if (line == "\r")
			break;
		line.pop_back();
		head += line + "\n";
	}

	stringstream req(head);
	
	getline(req, line);
	headerFirstLine = Request::getVector(line);

	if (headerFirstLine.size() != 3)
		goto error;

	while (getline(req, line))
	{
		tokens = getVector(line);

		string key = tokens[0];
		string value = line.substr(key.length() + 1);

		key.pop_back();
		headers.insert(make_pair(key, value));
	}

	// headerFirstLine[2].pop_back();
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
		request += it->first + it->second + endLine;	
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
	cout << "#" << _method << "#" << endl;
	return (_method != "GET" && _method != "DELETE" && _method != "POST");
}

bool Request::transferEncoding()
{
	return (_headers.find("Transfer-Encoding") != _headers.end());
}

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
	setUrlArgs();
	resource();

	// if ( syntaxError() ) 		return BAD_REQUEST;
	
	if (isMethodAllowed()) return METHOD_NOT_ALLOWED;
	
	if ( transferEncoding() )
	{
		if (_headers["Transfer-Encoding"] != "chunked")
			return NOT_IMPLEMENTED;
	}

	if (getMethod() == "POST")
	{
		if ( _headers.find("Content-Length") == _headers.end() && _headers.find("Transfer-Encoding") == _headers.end() )
			return BAD_REQUEST;
	}

	if ( !allowedChars() )		return BAD_REQUEST;
	if ( acceptUriLength() )	return REQUEST_URI_TOO_LONG;
	if ( isVersionSupported() )	return VERSION_NOT_SUPPORTED;

	return OK;
}

void Request::setUrlArgs()
{
	string url = _resource;
	
	size_t pos = url.find('?');
	if (pos == string::npos)
		return;
	
	_resource = url.substr(0, pos);
	_queryString = url.substr(pos + 1);
}

string Request::getQueryString()
{
	return _queryString;
}

bool Request::isUploadable()
{
    return _isUploadable;
}

string Request::getBody()
{
	return _body;
}

void Request::setBody(string request)
{
	_body = request;
}

bool 	Request::isHeaderHasKey(string key)
{
	return (_headers.find(key) != _headers.end());	
}

string 	Request::getValueOf(string key)
{
	return _headers[key];
}

map<string, string> Request::getHeader()
{
	return _headers;
}

void Request::setUploadable()
{
	_isUploadable = true;
}

void Request::uploadFile()
{
    size_t nextLine ;
    string boundary;
    string endBoundary;

    vector<string> contentType = Request::getVector(getValueOf("Content-Type"));
    
    boundary = "--" + contentType[1].substr(9);
    endBoundary = boundary + "--";

    size_t start = _body.find(boundary);
    size_t end = _body.find(endBoundary);

    if (start == string::npos || end == string::npos) // return msg error : bad request
    {
        cerr << "bad request" << endl;
        return;
    }
    
    string tmp;
    string bodyHead;
    vector <string> bodyHeadVector;
    
    while (start != string::npos && start < end)
    {
        nextLine = _body.find("\r\n\r\n") + 4;
        bodyHead = _body.substr(0, nextLine);

        while (bodyHead.find("\r\n") != string::npos)
            bodyHead.replace(bodyHead.find("\r\n"), 2, " ");

        bodyHeadVector = Request::getVector(bodyHead);
        while (bodyHeadVector.size() && bodyHeadVector[0].find("filename") == string::npos)
            bodyHeadVector.erase(bodyHeadVector.begin());

        if (!bodyHeadVector.size())
            return ;
        
        string filename = bodyHeadVector[0].substr(bodyHeadVector[0].find("\"") + 1);
        filename.erase(filename.find("\""));

        _body.erase(0, nextLine);
        start = _body.find(boundary);
        tmp = _body.substr(0, start);

        ofstream file("./uploads/" + filename, ios::out | ios::trunc);

        file << tmp;
        tmp.clear();
    }
}


Request::~Request() {}
