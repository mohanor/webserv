/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 22:22:26 by matef             #+#    #+#             */
/*   Updated: 2023/03/18 15:52:36 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef UTILITY_HPP
# define UTILITY_HPP

# include <iostream>
# include <fstream>
# include <cctype>
# include <vector>

using namespace std;

enum Method
{
    GET = 1 << 0,
    HEAD,
    POST,
    PUT,
    DELETE,
    TRACE,
    OPTIONS,
    CONNECT
};

std::string getFileContent(std::string file_name);
bool hasOnlyUppercase(const string &str);
bool syntaxIsCorrect(vector<string> first_line);
size_t getFileSize(std::string filename);

# endif