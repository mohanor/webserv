/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 22:03:04 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/03/05 21:30:03 by yoelhaim         ###   ########.fr       */
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
    // cout << _server[0]._location[0].first << endl;
    infoServer();
}

void Configuration::infoServer()
{
    int i = 0;
    while (i < _length_server)
    {
         cout << "-------------info server : " << i <<" ------------------\n";
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
        for (size_t i = index + 1; _tokens[i].second != SEMI_COLON ; i++)
        {
            indexPage += " " + _tokens[i].first;
        }
         _directive_server.push_back(make_pair(indexPage, INDEX));
    }
    if (_tokens[index].first == "error_page")
        _directive_server.push_back(make_pair(_tokens[index + 1].first + " "+_tokens[index + 2].first, ERROR_PAGE));
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
        string allow ;
        for (size_t i = index + 1; _tokens[i].second != SEMI_COLON ; i++)
        {
            allow += " " + _tokens[i].first;
        }
        cout << "allow " << allow << endl;
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

void Configuration::pushLocation(size_t index, size_t indexServer)
{
    cout << "push location => " << _server[0].getHost() << endl;
    //  if (_tokens[index].first == "listen")
    //     _server[indexServer]._location.push_back(make_pair("sjksj", LISTEN));
    // if (_tokens[index].first == "root")
    //      _server[indexServer]._location.push_back(make_pair(_tokens[index + 1].first, ROOT));
    // if (_tokens[index].first == "index")
    // {
    //     string indexPage;
    //     for (size_t i = index + 1; _tokens[i].second != SEMI_COLON ; i++)
    //     {
    //         indexPage += " " + _tokens[i].first;
    //           _server[indexServer]._location.push_back(make_pair(indexPage, INDEX));
    //     }
    // }
    // if (_tokens[index].first == "error_page")
    //      _server[indexServer]._location.push_back(make_pair(_tokens[index + 1].first + " "+_tokens[index + 2].first, ERROR_PAGE));
    // if (_tokens[index].first == "cli_max_size")
    //      _server[indexServer]._location.push_back(make_pair(_tokens[index + 1].first, CLIENT_MAX_BODY_SIZE));
    // if (_tokens[index].first == "autoindex")
    //      _server[indexServer]._location.push_back(make_pair(_tokens[index + 1].first, AUTOINDEX));
    // if (_tokens[index].first == "server_name")
    //      _server[indexServer]._location.push_back(make_pair(_tokens[index + 1].first, SERVER_NAME));
    // if (_tokens[index].first == "host")
    //      _server[indexServer]._location.push_back(make_pair(_tokens[index + 1].first, HOST));
    // if (_tokens[index].first == "allow")
    // {
    //     string allow ;
    //     for (size_t i = index + 1; _tokens[i].second != SEMI_COLON ; i++)
    //     {
    //         allow += " " + _tokens[i].first;
    //     }
    //     cout << "allow " << allow << endl;
    //     _directive_server.push_back(make_pair(allow, ALLOWED_METHODS));
    // }
}

size_t Configuration::getDirectiveLocation(size_t index , size_t indexServer)
{
    while (index < _tokens.size() && _tokens[index].second != CLOSE_CURLY)
    {
        if (_tokens[index].second == DIRECTIVE)
            pushLocation(index, indexServer);
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
                index = getDirectiveLocation(index, i);
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
                index++; lengtLocation = 0;
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
    // checkLocation();
}
