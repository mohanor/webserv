/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 21:42:58 by matef             #+#    #+#             */
/*   Updated: 2023/04/17 22:23:41 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utility.hpp"

std::string getFileContent(std::string file_name)
{
    std::string content, line;
    std::ifstream MyReadFile(file_name);

    if (!MyReadFile.is_open())
        return "";

    while (getline (MyReadFile, line))
        content += line + '\n';

    MyReadFile.close();
    return content.substr(0, content.size() - 1);
}

bool hasOnlyUppercase(const string &str)
{
    for (string::const_iterator it = str.begin(); it != str.end(); it++)
    {
        if (isalpha(*it) && islower(*it))
            return false;
    }
    return true;
}

bool syntaxIsCorrect(vector<string> first_line)
{
    vector<string>::iterator it = first_line.begin();
    
    for(; it != first_line.end() ; it++)
    {
        if (not hasOnlyUppercase(*it))
            return false;
    }
    return true;
}

size_t getFileSize(std::string filename) { return getFileContent(filename).size(); }

string getComment(int status)
{
    switch (status)
    {
        case 501:
            return "Not Implemented";
        case 414:
            return "Request-URI Too Long";
        case 413:
            return "Request Entity Too Large";
        default:
            return "Bad Request";
    }
}