/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 19:33:13 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/03/03 00:40:22 by yoelhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

//    <--------- function trim espace --------->
string ft_trim(string str)
{
    int lengthReverse = str.length();
    int i = 0;
    for (; str[i] && (str[i] == 32 || str[i] == '\t'); i++)
    {
    }
    for (; str[lengthReverse] && (str[lengthReverse] == 32 || str[lengthReverse] == '\t'); lengthReverse++)
    {
    }

    return (str.substr(i, lengthReverse));
}
string searchCommentInLine(string line)
{
    size_t i = 0;
    string comment = "";

    int s = line.find('#');
    return line.substr(0, s);
}

//    <--------- fin function trim espace --------->

//    <--------- constructor and destructor --------->
ConfigParser::ConfigParser()
{
    cout << "ConfigParser constructor" << endl;
}

ConfigParser::ConfigParser(string content)
{

    if (content.empty())
        errorLogs("Error: file is empty");
    _index = 0;
    vector<string> conf;
    conf = Request::getVector(content, '\n');

    for (size_t index = 0; index < conf.size(); index++)
        _lines.append(searchCommentInLine(ft_trim(conf[index])));

    // !! parse line by line and remove espace
    parseLine(_lines);
    // !! tokenez data and save it in vector
    tokenez();
    synaxError();
    insertServer();

    cout << "successfuly parse\n";
}

ConfigParser::~ConfigParser()
{
}

//    <--------- end constructor and destructor --------->

void ConfigParser::errorLogs(string titleError)
{
    cout << titleError << endl;
    exit(1);
}

bool checkLineIsComment(string line)
{
    size_t i = 0;

    for (; (line[i] == ' ' || line[i] == '\t') && line[i]; i++)
    {
    }
    if (line[i] == '#' || !strlen(line.c_str() + i))
        return true;
    return false;
}

// ?? push context yo tokonez

void ConfigParser::pushTokinez(int currentIndex, string context)
{
    int word;
    if (currentIndex != _index)
    {
        vector<string> ctx = Request::getVector(context);
        for (size_t i = 0; i < ctx.size(); i++)
        {
            //* check if context or word
            i == 0 ? word = CONTEXT : word = WORD;
            _tokens.push_back(make_pair(ctx[i], word));
        }
    }
}

void ConfigParser::pushSemiCurly(string type)
{
    int word;
    type == "}" ? word = CLOSE_CURLY : word = SEMI_COLON;
    _tokens.push_back(make_pair(type, word));
}

// !! check context and push it to vector _tokens
void ConfigParser::checkContext()
{
    string context;
    string tmp;
    size_t currentIndex = this->_index;

    while (_data[_index] != "{" && _index < _data.size())
    {
        tmp.append(_data[_index]).append(" ");
        if (_data[_index] == ";" || _data[_index] == "}")
        {
            checkDirectives(tmp);
            pushSemiCurly(_data[_index]);
            tmp.clear();
            context.clear();
        }
        else
            context.append(_data[_index]).append(" ");
        _index++;
    }
    pushTokinez(currentIndex, context);
}

void ConfigParser::pushDirective(string directive)
{
    int word;
    vector<string> direct = Request::getVector(directive);
    for (size_t i = 0; i < direct.size(); i++)
    {
        // * check first word is directev and second is word;
        i == 0 ? word = DIRECTIVE : word = WORD;
        _tokens.push_back(make_pair(direct[i], word));
    }
}

// !! check  Directive and push it to vector _tokens
void ConfigParser::checkDirectives(string data)
{
    string directive;

    for (size_t i = 0; i < data.size() && data[i] != ';'; i++)
    {
        if (data[i] == '}')
        {
            i++;
            continue;
        }
        directive.append(data, i, 1);
    }

    if (directive.size() != 0)
        pushDirective(directive);
}

// !! tokenez the data and push it to vector _tokens
void ConfigParser::tokenez()
{

    for (_index = 0; _index < _data.size(); _index++)
    {
        checkContext();
        if (_data[_index] == "{")
            _tokens.push_back(make_pair("{", OPEN_CURLY));
    }

    // for (size_t i = 0; i < _tokens.size(); i++)
    // {
    //    cout << _tokens[i].first << "  " << _tokens[i].second<< endl;
    // }
}

// !! check  Punctuation ;
void ConfigParser::checkPunctuation(string line)
{
    if (line[_index] == ';')
    {
        _data.push_back(";");
        _index++;
    }
}

// !! check  open curly breacts  {
void ConfigParser::checkOpenCurly(string line)
{
    if (line[_index] == '{')
    {
        _data.push_back("{");
        _index++;
    }
}

// !! check  close curly breacts }
void ConfigParser::checkCloseCurly(string line)
{

    if (line[_index] == '}')
    {
        _data.push_back("}");
        _index++;
    }
}

// !! check espace and remove duplicated espace
void ConfigParser::skipSpaces(string line)
{
    while (line[_index] && (line[_index] == ' ' || line[_index] == '\t'))
        _index++;
}

// !! check  word
void ConfigParser::checkWord(string line)
{
    int currentIndex = _index;
    string word;

    while (line[_index] && !strchr(" \t{};", line[_index]))
    {
        word.append(1, line[_index]);
        _index++;
    }

    if (currentIndex != _index)
        this->_data.push_back(word);
}

void ConfigParser::parseLine(string line)
{

    while (line[_index])
    {
        skipSpaces(line);
        checkWord(line);
        checkOpenCurly(line);
        checkCloseCurly(line);
        checkPunctuation(line);
    }
}

// ******** start syntax Error ********
void ConfigParser::checkSynatxCurly()
{
    size_t countCurly = 0;

    for (size_t i = 0; i < _tokens.size(); i++)
    {
        if (_tokens[i].second == OPEN_CURLY || _tokens[i].second == CLOSE_CURLY)
            countCurly++;
    }
    if (_tokens[_tokens.size() - 1].second != CLOSE_CURLY)
        errorLogs("Error find closed curly");
    if (countCurly % 2 != 0)
        errorLogs("Error find closed curly");
}

void ConfigParser::checkSyntaxMain()
{

    if (_tokens[0].first != "server" || _tokens[1].second != OPEN_CURLY || _tokens[2].second == OPEN_CURLY)
        errorLogs("error find server");
}
void ConfigParser::checkSyntaxContext()
{
    for (size_t i = 1; i < _tokens.size(); i++)
    {
        if (_tokens[i].second == CONTEXT && _tokens[i + 1].second == OPEN_CURLY)
            if (_tokens[i].first != "server")
                errorLogs("error server synatx");

        if (_tokens[i].second == CONTEXT && _tokens[i + 1].second != OPEN_CURLY)
        {
            if (_tokens[i].first != "location" || (_tokens[i + 1].second != WORD && _tokens[i + 2].second != OPEN_CURLY))
                errorLogs("error Location synatx");
        }
    }
}

bool ConfigParser::checkSyntaxDirectiveCondition(size_t index)
{
    int i = 0;

    for (; (_tokens[index].second != SEMI_COLON) && index < _tokens.size(); index++, i++)
    {
    }

    if (_tokens[index].second != SEMI_COLON)
        errorLogs("error word");
    return true;
}
size_t ConfigParser::lengthDirective(size_t index)
{
    size_t i = 0;
    for (; _tokens[index].second != SEMI_COLON && index < _tokens.size(); index++, i++)
    {
    }
    return i;
}

void ConfigParser::checkMaxTwo(size_t index)
{
    if (_tokens[index].first != "allow" && _tokens[index].first != "error_page" && _tokens[index].first != "return" && _tokens[index].first != "index")
        if (lengthDirective(index) != 2)
            errorLogs("error variable  " + _tokens[index].first);
}

void ConfigParser::checkSyntaxCemiColom()
{

    bool checkCemiColom = true;
    for (size_t i = 0; i < _tokens.size(); i++)
    {
        if (_tokens[i].second == DIRECTIVE)
        {
            if (_tokens[i].first == "allow" && lengthDirective(i) > 4)
                errorLogs("error allow");
            else if (_tokens[i].first == "error_page" && lengthDirective(i) != 3)
                errorLogs("error error page");
            else if (_tokens[i].first == "index" && lengthDirective(i) > 4)
                errorLogs("error index page");
            else if (_tokens[i].first == "return" && lengthDirective(i) != 3)
                errorLogs("error return");
            checkMaxTwo(i);
        }
    }
}

void ConfigParser::checkSyntaxDirective()
{
    string myDirective[10] = {"server_name", "listen", "allow", "autoindex", "index", "error_page", "return", "host", "root", "cli_max_size"};

    size_t i = 0;
    for (; i < _tokens.size(); i++)
    {
        if (_tokens[i].second == DIRECTIVE)
        {
            checkSyntaxDirectiveCondition(i);
            size_t j = 0;
            for (; j < 10; j++)
                if (myDirective[j] == _tokens[i].first)
                    break;
            if (j == 10)
                errorLogs("error variable  " + _tokens[i].first);
        }
    }
}

void ConfigParser::synaxError()
{
    checkSynatxCurly();
    checkSyntaxMain();
    checkSyntaxContext();
    checkSyntaxDirective();
    checkSyntaxCemiColom();
}

// ******** end    syntax Error ********

// ******** start  Insert ********

void ConfigParser::addToServer(size_t index, int indexServer, vector<Server> _server)
{
    if (_tokens[index].first == "listen")
        _server[indexServer] = Server(10000,"sss", "sss", "sss","kjkj", true, 4444, "sss", "s");
    // _server[indexServer].setListen(100000);
    // if (_tokens[index].first == "listen")
    // {
    //     cout << atoi(_tokens[index + 1].first.c_str())<< " "<< indexServer <<"\n";
    //     _server[indexServer].setListen(atoi(_tokens[index + 1].first.c_str()));
    // }
    // if (_tokens[index].first == "server_name")
    //     _server[indexServer].setServerName(_tokens[index + 1].first);
    // if (_tokens[index].first == "host")
    //     _server[indexServer].setHost(_tokens[index + 1].first);
    // if (_tokens[index].first == "root")
    //     _server[indexServer].setRoot(_tokens[index + 1].first);
    // if (_tokens[index].first == "autoindex")
    //     _server[indexServer].setAutoIndex(_tokens[index + 1].first == "on" ? true : false);
    // if (_tokens[index].first == "index")
    //     _server[indexServer].setIndex(_tokens[index + 1].first);
    // if (_tokens[index].first == "error_page")
    //     _server[indexServer].setErrorPage(_tokens[index + 1].first + " " + _tokens[index + 2].first);
}

size_t ConfigParser::numberServer() const
{
    int numberOfServer = 0;
    for (size_t i = 0; i < _tokens.size(); i++)
    {
        _tokens[i].first == "server" && _tokens[i + 1].first == "{" ? numberOfServer++ : numberOfServer;
    }
    return (numberOfServer);
}

void ConfigParser::insertServer()
{
    size_t index = 1;

    // vector<Server> _server;
    for (size_t i = 0; i < numberServer(); i++)
    {
       _server.push_back(Server());
    }
    

    for (size_t i = 0; i < numberServer(); i++)
    {
        bool cxt;

        
        while (index < _tokens.size())
        {
            if (_tokens[index].first == "server")
            {
                index++;
                break;
            }
            if (_tokens[index].second == CONTEXT && _tokens[index + 1].second == WORD)
                cxt = true;
            if (_tokens[index].second == DIRECTIVE && !cxt)
                addToServer(index, i, _server);
            if (_tokens[index].second == CLOSE_CURLY)
                cxt = false;
            index++;
        }
    }
}

// ******** end  Insert ********

void ConfigParser::printData()
{

    // for (size_t i = 0; i < _tokens.size(); i++)
    // {
    //     cout << _tokens[i].first << " =>" << _tokens[i].second << endl;
    // }

    for (size_t i = 0; i < _server.size(); i++)
    {
       cout << _server[i].getListen() << endl;
    }
    
    cout << numberServer() << endl;
}
