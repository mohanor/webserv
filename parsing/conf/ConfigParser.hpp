/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 19:30:47 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/03/08 23:23:49 by yoelhaim         ###   ########.fr       */
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
using namespace std;

class ConfigParser
{
private:
    string _lines;
    size_t _index;
    size_t _lenght_server;
    vector<string> _data;
    vector<pair<string, int> > _tokens;
    
public:
    ConfigParser();
    ConfigParser(string content);
    ConfigParser(const ConfigParser &src);
    ConfigParser &operator=(const ConfigParser &src);
    ~ConfigParser();
    void printData();
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

    void pushTokinez(int currentIndex, string context);
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

    void checkSyntaxDiplicated();
    void checkSyntaxDiplicatedLocation(size_t index,map<string, bool> &directive);
    size_t lengthDirective(size_t index);
    void  lenghtServer(int index);
    int getLengthServer() const;
    vector<pair<string, int> > getTokens() const;
    vector<Server>  getServers();
    map<string , Location> getLocation();
  
};
