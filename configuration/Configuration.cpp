/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 22:03:04 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/04/12 20:26:28 by yoelhaim         ###   ########.fr       */
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
    

    map<short int, bool> listen_port;
    for (size_t i = 0; i < _server.size(); i++)
    {
        vector<short int> listen = _server[i].getPort();

        for (size_t i = 0; i < listen.size(); i++)
        {
            if (listen_port.find(listen[i]) != listen_port.end())
            {
                cerr << "Error : Port is diplicated" << endl;
                exit(1);
            }
            else
                listen_port.insert(make_pair(listen[i], false));
        }
         listen_port.clear();
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
        _directive_server.push_back(make_pair(_tokens[index + 1].first, LISTEN));
    if (_tokens[index].first == "host")
        _directive_server.push_back(make_pair(_tokens[index + 1].first, HOST));
    if (_tokens[index].first == "root")
        _directive_server.push_back(make_pair(_tokens[index + 1].first, ROOT));
    if (_tokens[index].first == "index")
    {
        string indexPage;
        for (size_t i = index + 1; _tokens[i].second != SEMI_COLON; i++)
            indexPage += " " + _tokens[i].first;
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
            allow += " " + _tokens[i].first;

        _directive_server.push_back(make_pair(allow, ALLOWED_METHODS));
    }
    if (_tokens[index].first == "cgi_info_php")
    {
        string cgi_info;
        for (size_t i = index + 1; _tokens[i].second != SEMI_COLON; i++)
            cgi_info += " " + _tokens[i].first;

        _directive_server.push_back(make_pair(cgi_info, CGI_INFO_PHP));
    }
    if (_tokens[index].first == "cgi_info_py")
    {
        string cgi_info;
        for (size_t i = index + 1; _tokens[i].second != SEMI_COLON; i++)
            cgi_info += " " + _tokens[i].first;

        _directive_server.push_back(make_pair(cgi_info, CGI_INFO_PYTHON));
    }
}

void Configuration::addToServer()
{
    directives directive;

    for (size_t i = 0; i < _directive_server.size(); i++)
    {
        switch (_directive_server[i].second)
        {
        case LISTEN:
            directive.listen.push_back(atoi(_directive_server[i].first.c_str()));
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
        {
            vector<string> errorPage = Request::getVector(_directive_server[i].first);
            directive.error_page.push_back(make_pair(atoi(errorPage[0].c_str()), errorPage[1]));
            break;
        }
        case AUTOINDEX:
            directive.autoindex = _directive_server[i].first == "on" ? true : false;
            break;
        case ALLOWED_METHODS:
            directive.allow = _directive_server[i].first;
            break;
        case CGI_INFO_PHP:
            directive.cgi_info_php = _directive_server[i].first;
            break;
        case CGI_INFO_PYTHON:
            directive.cgi_info_py = _directive_server[i].first;
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
            allow += " " + _tokens[i].first;
        _server[indexServer]._location.push_back(make_pair(allow, ALLOWED_METHODS));
    }
    if (_tokens[index].first == "cgi_info_php")
    {
        string cgi_info;
        for (size_t i = index + 1; _tokens[i].second != SEMI_COLON; i++)
            cgi_info += " " + _tokens[i].first;

        _server[indexServer]._location.push_back(make_pair(cgi_info, CGI_INFO_PHP));
    }
    if (_tokens[index].first == "cgi_info_py")
    {
        string cgi_info;
        for (size_t i = index + 1; _tokens[i].second != SEMI_COLON; i++)
            cgi_info += " " + _tokens[i].first;

        _server[indexServer]._location.push_back(make_pair(cgi_info, CGI_INFO_PYTHON));
    }
    if (_tokens[index].first == "upload_store")
        _server[indexServer]._location.push_back(make_pair(_tokens[index + 1].first, UPLOAD_STORE));
    if (_tokens[index].first == "upload_enable")
        _server[indexServer]._location.push_back(make_pair(_tokens[index + 1].first, UPLOAD_ENABLE));
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
    case UPLOAD_STORE:
        return "upload_store";
    case UPLOAD_ENABLE:
        return "upload_enable";
    case CGI_INFO_PHP:
        return "cgi_info_php";
    case CGI_INFO_PYTHON:
        return "cgi_info_py";
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

        if (getKey(indexOfKey) == "error_page")
        {
            vector<string> errorPage = Request::getVector(value);
            directive.error_page_location.insert(make_pair(errorPage[0], errorPage[1]));
        }
        else
            directive._directives[getKey(indexOfKey)] = value;
        i++;
    }

    directive.path = nameLocation;
    _server[index].setLocation(directive.path, directive);
}

size_t Configuration::getDirectiveLocation(size_t index, size_t indexServer)
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
                index = getDirectiveLocation(index, i);
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
