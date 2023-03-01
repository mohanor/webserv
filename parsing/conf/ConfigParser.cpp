/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 19:33:13 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/03/01 17:52:31 by yoelhaim         ###   ########.fr       */
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
    string word;
    if (currentIndex != _index)
    {
        vector<string> ctx = Request::getVector(context);
        for (size_t i = 0; i < ctx.size(); i++)
        {
            //* check if context or word
            i == 0 ? word = "context" : word = "word";
            _tokens.push_back(make_pair(ctx[i], word));
        }
    }
}

void ConfigParser::pushSemiCurly(string type)
{
    string word;
    type == "}" ? word = "closeCurly" : word = "semiColom";
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
    string word;
    vector<string> direct = Request::getVector(directive);
    for (size_t i = 0; i < direct.size(); i++)
    {
        // * check first word is directev and second is word;
        i == 0 ? word = "directive" : word = "word";
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
            _tokens.push_back(make_pair("{", "openCurly"));
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
        if (_tokens[i].second == "openCurly" || _tokens[i].second == "closeCurly")
            countCurly++;
    }
    if (countCurly % 2 != 0)
        errorLogs("Error find closed curly");
}

void ConfigParser::checkSyntaxMain()
{
    if (_tokens[0].first != "server" && _tokens[1].second != "openCurly")
        errorLogs("error find server");
}
void ConfigParser::checkSyntaxContext()
{
    for (size_t i = 1; i < _tokens.size(); i++)
    {

        if (_tokens[i].second == "context")
        {
            if (_tokens[i].first != "location" || (_tokens[i + 1].second != "word" && _tokens[i + 2].second != "openCurly"))
                errorLogs("error Location synatx");
        }
    }
}

bool ConfigParser::checkSyntaxDirectiveCondition(size_t index)
{
    int i = 0;
   
   for (; _tokens[index].second != "semiColom" && index <  _tokens.size();index++,i++){}
   if (i == 1)
        errorLogs("error word");
    return true;
}

void ConfigParser::checkSyntaxDirective()
{
    string myDirective[10] = {"server_name", "listen", "allow", "autoindex", "index", "error_page", "return", "host", "root", "cli_max_size"};

    size_t i = 0;
    for (; i < _tokens.size(); i++)
    {
        if (_tokens[i].second == "directive")
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
}

// ******** end    syntax Error ********

void ConfigParser::printData()
{
    cout << _lines << endl;
}
