/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 22:03:04 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/03/19 16:10:03 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Configuration.hpp"

Configuration::Configuration(string fileName)
{
    this->_config = ConfigParser(fileName);

    this->_tokens = _config.getTokens();
    this->_length_server = _config.getLengthServer();

    insertServer();
    checkLocation();

    map<std::string, bool> checkServerName;
    for (size_t i = 0; i < _server.size(); i++)
    {
        if (checkServerName.find(_server[i].getServerName()) != checkServerName.end())
        {
           cerr <<  "Error: ServerName already in use" << endl;
              exit(1);
        }
        checkServerName[_server[i].getServerName()] = false;
    }
}

Configuration::~Configuration()
{
}

Configuration::Configuration(const Configuration &copy)
{
    *this = copy;
}

Configuration &Configuration::operator=(const Configuration &copy)
{
    if (this != &copy)
    {
        this->_config = copy._config;
        this->_tokens = copy._tokens;
        this->_length_server = copy._length_server;
        this->_server = copy._server;
        this->_directive_server = copy._directive_server;
    }
    return *this;
}

void Configuration::checkDirective(size_t index)
{
    if (_tokens[index].first == "listen")
    {
        _directive_server.push_back(make_pair(_tokens[index + 1].first, LISTEN));
        _directive_server.push_back(make_pair(_tokens[index + 2].first, HOST));       
    }
    if (_tokens[index].first == "root")
        _directive_server.push_back(make_pair(_tokens[index + 1].first, ROOT));
    if (_tokens[index].first == "index")
    {
        string indexPage;
        for (size_t i = index + 1; _tokens[i].second != SEMI_COLON; i++)
        {
            indexPage += " " + _tokens[i].first;
        }
        _directive_server.push_back(make_pair(indexPage, INDEX));
    }
    if (_tokens[index].first == "error_page")
        _directive_server.push_back(make_pair(_tokens[index + 1].first + " " + _tokens[index + 2].first, ERROR_PAGE));
    if (_tokens[index].first == "cli_max_size")
        _directive_server.push_back(make_pair(_tokens[index + 1].first, CLIENT_MAX_BODY_SIZE));
    if (_tokens[index].first == "autoindex")
        _directive_server.push_back(make_pair(_tokens[index + 1].first, AUTOINDEX));
    if (_tokens[index].first == "server_name")
        _directive_server.push_back(make_pair(_tokens[index + 1].first, SERVER_NAME));
    if (_tokens[index].first == "allow")
    {
        string allow;
        for (size_t i = index + 1; _tokens[i].second != SEMI_COLON; i++)
        {
            allow += " " + _tokens[i].first;
        }
        _directive_server.push_back(make_pair(allow, ALLOWED_METHODS));
    }
    // map<string, bool>::iterator it = checkPorts.begin();

   
    
}

void Configuration::addToServer()
{
    directives directive;

    for (size_t i = 0; i < _directive_server.size(); i++)
    {
        switch (_directive_server[i].second)
        {
        case LISTEN:
            directive.listen = atoi(_directive_server[i].first.c_str());
            break;
        case SERVER_NAME:
            directive.server_name = _directive_server[i].first;
            break;
        case HOST:
            directive.host = _directive_server[i].first;
            break;
        case CLIENT_MAX_BODY_SIZE:
            directive.cli_max_size = atoi(_directive_server[i].first.c_str());
            break;
        case ROOT:
            directive.root = _directive_server[i].first;
            break;
        case INDEX:
            directive.index = _directive_server[i].first;
            break;
        case ERROR_PAGE:
            directive.error_page = _directive_server[i].first;
            break;
        case AUTOINDEX:
            directive.autoindex = _directive_server[i].first == "on" ? true : false;
            break;
        case ALLOWED_METHODS:
            directive.allow = _directive_server[i].first;
            break;
        }
    }
    _server.push_back(Server(directive));
}

void Configuration::checkDirectiveLocation(size_t index, size_t indexServer)
{
    if (_tokens[index].first == "root")
        _server[indexServer]._location.push_back(make_pair(_tokens[index + 1].first, ROOT));
    if (_tokens[index].first == "index")
    {
        string indexPage;
        for (size_t i = index + 1; _tokens[i].second != SEMI_COLON; i++)
        {
            indexPage += " " + _tokens[i].first;
            _server[indexServer]._location.push_back(make_pair(indexPage, INDEX));
        }
    }
    if (_tokens[index].first == "return")
        _server[indexServer]._location.push_back(make_pair(_tokens[index + 1].first + " " + _tokens[index + 2].first, RETURN));
    if (_tokens[index].first == "error_page")
        _server[indexServer]._location.push_back(make_pair(_tokens[index + 1].first + " " + _tokens[index + 2].first, ERROR_PAGE));
    if (_tokens[index].first == "cli_max_size")
        _server[indexServer]._location.push_back(make_pair(_tokens[index + 1].first, CLIENT_MAX_BODY_SIZE));
    if (_tokens[index].first == "autoindex")
        _server[indexServer]._location.push_back(make_pair(_tokens[index + 1].first, AUTOINDEX));
    if (_tokens[index].first == "allow")
    {
        string allow;
        for (size_t i = index + 1; _tokens[i].second != SEMI_COLON; i++)
        {
            allow += " " + _tokens[i].first;
        }
        _server[indexServer]._location.push_back(make_pair(allow, ALLOWED_METHODS));
    }
}

string Configuration::getKey(int index)
{
    switch (index)
    {
    case CLIENT_MAX_BODY_SIZE:
        return "cli_max_size";
    case ROOT:
        return "root";
    case INDEX:
        return "index";
    case ERROR_PAGE:
        return "error_page";
    case AUTOINDEX:
        return "autoindex";
    case ALLOWED_METHODS:
        return "allow";
    default:
        return "return";
    }
}

void Configuration::pushLocation(size_t index, string nameLocation)
{
    Location directive;
    size_t i = 0;

    while (i < _server[index]._location.size())
    {
        int indexOfKey = _server[index]._location[i].second;
        string value = _server[index]._location[i].first;
        directive._directives[getKey(indexOfKey)] = value;
        i++;
    }

    directive.path = nameLocation;
    _server[index].setLocation(directive.path , directive);
    
}

size_t Configuration::getDirectiveLocation(size_t index, size_t indexServer, string nameLocation)
{
    while (index < _tokens.size() && _tokens[index].second != CLOSE_CURLY)
    {
        if (_tokens[index].second == DIRECTIVE)
            checkDirectiveLocation(index, indexServer);
        index++;
    }

    return index;
}

void Configuration::checkLocation()
{
    size_t index = 1;

    for (size_t i = 0; i < _length_server; i++)
    {
        while (index < _tokens.size())
        {
            if (_tokens[index].first == "server")
            {
                index++;
                break;
            }
            if (_tokens[index].second == CONTEXT && _tokens[index + 1].second == WORD)
            {
                string nameLocation = _tokens[index + 1].first;
                index = getDirectiveLocation(index, i, nameLocation);
                pushLocation(i, nameLocation);
                _server[i]._location.clear();
            }
            index++;
        }
    }
}

void Configuration::insertServer()
{
    size_t index = 1;
    size_t lengtLocation = 0;

    for (size_t i = 0; i < _length_server; i++)
    {
        bool cxt = false;

        while (index < _tokens.size())
        {
            if (_tokens[index].first == "server")
            {
                index++;
                lengtLocation = 0;
                break;
            }
            if (_tokens[index].second == CONTEXT && _tokens[index + 1].second == WORD)
            {
                lengtLocation++;
                cxt = true;
            }
            if (_tokens[index].second == DIRECTIVE && !cxt)
                checkDirective(index);
            if (_tokens[index].second == CLOSE_CURLY)
                cxt = false;
            index++;
        }
        addToServer();
        _directive_server.clear();
    }
}

vector<Server> Configuration::getServers() const
{
    return _server;
}
