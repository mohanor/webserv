/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 02:26:41 by matef             #+#    #+#             */
/*   Updated: 2023/02/28 00:18:50 by matef            ###   ########.fr       */
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
        v.push_back(sub);
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

	if (headerFirstLine.size() != 3 || not syntaxIsCorrect(headerFirstLine) )
		goto error;


	while (getline(requestAsFile, line))
	{
		tokens = getVector(line);
		
		string key = tokens[0].substr(0, tokens[0].length() - 1);
		string value = line.substr(key.length() + 1);

		headers.insert(make_pair(key, Header(key, value)));
	}

	return Request(headerFirstLine[0], headerFirstLine[1], headerFirstLine[2], headers);

	error:
		cerr << request << "\n" << request.size() << " " << request.length() << '\n';
		return Request("", "", "");
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
