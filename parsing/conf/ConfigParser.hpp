/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 19:30:47 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/03/02 23:09:42 by yoelhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <string>
#include "../request/Request.hpp"
#include "Server.hpp"
#include "Http.hpp"
using namespace std;

enum e_token
{
    OPEN_CURLY,
    CLOSE_CURLY,
    SEMI_COLON,
    WORD,
    DIRECTIVE,
    CONTEXT,
    WSPACE,
    END
};


class ConfigParser
{
private:
    string _lines;
    size_t _index;
    vector<string> _data;
    vector< pair<string, int> > _tokens;
    vector<Server> _server;
    
public:
    ConfigParser();
    ConfigParser(string content);
    void printData();
    ~ConfigParser();

    string getLineParses(vector<string> data);

    void parseLine(string line);

    void checkWord(string  line);

    // !! open and close curly brackets
    void checkOpenCurly(string  line);
    void checkCloseCurly(string  line);
    
    // !! Punctuation
    void checkPunctuation(string  line);
    
    // !! wspace
    void skipSpaces(string line);

    // !! tokenez line  and check errors syntax 
     void tokenez();
    //  !! check  Context
    vector<string>::iterator  checkContext2(vector<string>::iterator it);
    void checkContext();

    void pushTokinez(int currentIndex, string context);
    void pushSemiCurly(string type);
    // !! check  Directive
    void checkDirectives(string data);
    void pushDirective(string directive);
     
    // !! check  errors log
    void errorLogs(string titleError);
    // !! syntax error
    void synaxError();
    void checkSynatxCurly();
    void checkSyntaxMain();
    void checkSyntaxContext();
    void checkSyntaxDirective();
    bool checkSyntaxDirectiveCondition(size_t index);
    void checkSyntaxCemiColom();
    size_t lengthDirective(size_t index);
    void checkMaxTwo(size_t index);
    // ??Check and  push to Server
    void   insertServer();
    void checkAndInitail(size_t index, int indexServer,vector<Server> _server);
    void addToServer(size_t index, int indexServer,vector<Server> _server);
    size_t numberServer() const;
};
