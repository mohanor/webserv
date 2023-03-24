/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 05:05:54 by matef             #+#    #+#             */
/*   Updated: 2023/03/21 22:24:30 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "MimeTypes.hpp"

MimeTypes::MimeTypes(string file)
{
    parseMimeTypes(file);
}

MimeTypes::~MimeTypes()
{
}

MimeTypes &MimeTypes::operator=(const MimeTypes &rhs)
{
    if (this != &rhs)
    {
        this->mimeTypes = rhs.mimeTypes;
    }
    return *this;
}

MimeTypes::MimeTypes(const MimeTypes &rhs)
{
    *this = rhs;
}

string MimeTypes::getMimeType(string extension)
{
    return mimeTypes[extension];
}

map<string, string> MimeTypes::getMimeTypes() const
{
    return mimeTypes;
}

bool MimeTypes::isSemicolonInLast(string extension)
{
    return (extension.length() > 1 && extension[extension.length() - 1] == ';');
}

bool MimeTypes::isSyntaxValid(string file)
{
    bool firstLine = true;
    lineCounter = 0;
    bool breakLine = false;
    
    fstream f(file.c_str(), fstream::in);

    if (!f.is_open())
    {
        cout << "Error: mime type is open" << endl;
        return false;
    }

    string line;

    while (getline(f, line))
    {
        lineCounter++;
        vector<string> tokens =  Request::getVector(line);

        if (firstLine)
        {
            if (tokens.size() != 2 || tokens[0] != "types" || tokens[1] != "{" || line.find('}') != string::npos)
                return false;

            firstLine = false;
        }
        else
        {
            if (tokens.size() == 1)
            {
                if (tokens[0] == "}" && tokens[0].length() == 1 && !getline(f, line))
                {
                    breakLine  = true;
                    break;
                }
                return false;
            }
            else
            {
                if (!allowedChars(line)) return false;
                if (tokens.size() >= 2 && ( !isSemicolonInLast(tokens[tokens.size() - 1]) || line.find('}') != string::npos ))
                    return false;
            }
        }
        
    }

    f.close();
    return breakLine;
}

bool MimeTypes::allowedChars(string line)
{
    string specialChars = "./_- ";
    if (line.length() == 0) return false;

    size_t i = 0;
    while (i < line.length() - 1)
    {
        if ( isalpha(line[i]) || isdigit(line[i]) || specialChars.find(line[i]) != string::npos)
        {
            i++;
            continue;   
        }
        return false;
    }
    return true;
}

void MimeTypes::parseMimeTypes(string file)
{

    if (!isSyntaxValid(file))
    {
        cout << "mime.types : syntax error at line " << lineCounter << endl;
        exit (EXIT_FAILURE);
    }

    fstream f(file.c_str(), fstream::in);
    
    if (!f.is_open())
    {
        cout << "Error: mime type file: can't open" << endl;
        return ;
    }
    
    string line;
    getline(f, line);
    
    while (getline(f, line))
    {
        vector<string> tokens =  Request::getVector(line);
        
        string mimeType = tokens[0];
        for (size_t i = 1; i < tokens.size(); i++)
        {
            if (i == tokens.size() - 1) tokens[i].erase(tokens[i].length() - 1);
            mimeTypes[tokens[i]] = mimeType;
        }
    }

    f.close();
}

string MimeTypes::getExtension(string file)
{
    vector<string> splt = Request::getVector(file, '.');
    
    if (splt.size() == 1) return "";
    return (splt.back());
}