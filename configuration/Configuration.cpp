/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 22:03:04 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/03/07 20:59:11 by yoelhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Configuration.hpp"

Configuration::Configuration()
{
}

Configuration::~Configuration()
{
}

Configuration::Configuration(vector<pair<string, int> > _tokens, size_t length)
{
    cout << "------ constructor configuration ----- \n";
    this->_tokens = _tokens;
    this->_length_server = length;
    insertServer();

}

void Configuration::infoServer()
{
    int i = 0;
    while (i < _length_server)
    {
        cout << "-------------info server : " << i << " ------------------\n";
        cout << "server name: " << _server[i].getServerName() << endl;
        cout << "host: " << _server[i].getHost() << endl;
        cout << "listen: " << _server[i].getListen() << endl;
        cout << "root: " << _server[i].getRoot() << endl;
        cout << "index: " << _server[i].getIndex() << endl;
        cout << "error_page: " << _server[i].getErrorPage(1) << endl;
        cout << "allowed_methods: " << _server[i].getAllowedMethods(1) << endl;
        cout << "autoindex: " << _server[i].getAutoIndex() << endl;
        cout << "client_max_body_size: " << _server[i].getMaxSize() << endl;
        cout << "--------------------------------------\n";

        i++;
    }

     for (size_t i = 0; i < _server.size(); i++)
    {
        map<string, Location>::iterator it = _server[i]._locations.begin();
        while (it != _server[i]._locations.end())
        {
            cout << "name   : " << it->first <<  endl;

            map<string, string>::iterator it2 = it->second._directives.begin();
            while (it2 != it->second._directives.end())
            {
                cout << "key: " << it2->first << " value: " << it2->second << endl;
                it2++;
            }
            cout << "-------------\n";
            it++;
        }

        cout << "-------------\n";
    }
}
void Configuration::checkDirective(size_t index)
{

    if (_tokens[index].first == "listen")
        _directive_server.push_back(make_pair(_tokens[index + 1].first, LISTEN));
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
    if (_tokens[index].first == "host")
        _directive_server.push_back(make_pair(_tokens[index + 1].first, HOST));
    if (_tokens[index].first == "allow")
    {
        string allow;
        for (size_t i = index + 1; _tokens[i].second != SEMI_COLON; i++)
        {
            allow += " " + _tokens[i].first;
        }
        _directive_server.push_back(make_pair(allow, ALLOWED_METHODS));
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
        directive._directives.insert(make_pair(getKey(indexOfKey), value));
        i++;
    }

    directive.path = nameLocation;
    _server[index]._locations.insert(make_pair(directive.path, directive));
}

size_t Configuration::getDirectiveLocation(size_t index, size_t indexServer, string nameLocation)
{
    while (index < _tokens.size() && _tokens[index].second != CLOSE_CURLY)
    {
        if (_tokens[index].second == DIRECTIVE)
        {
            checkDirectiveLocation(index, indexServer);
        }

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
    checkLocation();
}
