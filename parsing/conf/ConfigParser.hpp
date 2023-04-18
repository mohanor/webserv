/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 19:30:47 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/04/18 07:00:41 by yoelhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string>
#include "../request/Request.hpp"
#include "Server.hpp"
#include "Http.hpp"
#include <fstream>
#define STATUSCODE "201 204 301 400 401 403 404 405 408 413 414 500 501 502 503 504 505"

using namespace std;

class ConfigParser
{
private:
    string _lines;
    size_t _index;
    size_t _lenght_server;
    vector<string> _data;
    vector<pair<string, int> > _tokens;
    vector<string> _directive_allowed;
    
public:
    ConfigParser();
    ConfigParser(string content);
    ConfigParser(const ConfigParser &src);
    ConfigParser &operator=(const ConfigParser &src);
    ~ConfigParser();
    void initalConfig(string content);

    void parseLine(string line);
    void checkWord(string  line);
    void checkOpenCurly(string  line);
    void checkCloseCurly(string  line);
    void checkPunctuation(string  line);
    void skipSpaces(string line);
    void checkNewLine(string line);
     void tokenez();
    vector<string>::iterator  checkContext2(vector<string>::iterator it);
    void checkContext();

    void pushTokinez(size_t currentIndex, string context);
    void pushSemiCurly(string type);
    void checkDirectives(string data);
    void pushDirective(string directive);
    void errorLogs(string titleError);
    void synaxError();
    void checkSynatxCurly();
    void checkSyntaxMain();
    void checkSyntaxContext();
    void checkSyntaxDirective();
    bool checkSyntaxDirectiveCondition(size_t index);
    void checkSyntaxCemiColom();
    void checkSyntaxAfterCemiColom();
    void checkSyntackInServer(int currentIndex,int index);
    void checkSynaxDirective();
    void checkSyntaxMethod(size_t index);
    void checkSyntaxReturn(size_t index);
    void checkValidValue(size_t n);
    void checkUploadStore(size_t index);

    void checkSyntaxDiplicated();
    void checkSyntaxDiplicatedLocation(size_t index,map<string, bool> &directive);
    size_t lengthDirective(size_t index);
    void  lenghtServer(size_t index);
    int getLengthServer() const;
    vector<pair<string, int> > getTokens() const;
     void checkCorrectSyntaxDirective(size_t index);
   void SetAllowedDirective(bool isInServer);
   void insetErrorPage();
  
};
