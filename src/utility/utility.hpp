/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 22:22:26 by matef             #+#    #+#             */
/*   Updated: 2023/04/02 23:36:25 by yel-khad         ###   ########.fr       */

/*                                                                            */
/* ************************************************************************** */

# ifndef UTILITY_HPP
# define UTILITY_HPP

# include <iostream>
# include <fstream>
# include <cctype>
# include <vector>

using namespace std;


std::string getFileContent(std::string file_name);
bool hasOnlyUppercase(const string &str);
bool syntaxIsCorrect(vector<string> first_line);
size_t getFileSize(std::string filename);
string getComment(int status);
# endif