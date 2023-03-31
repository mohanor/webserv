/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-khad <yel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 22:22:26 by matef             #+#    #+#             */
/*   Updated: 2023/03/30 20:13:59 by yel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef UTILITY_HPP
# define UTILITY_HPP

# include <iostream>
# include <fstream>
# include <cctype>
# include <vector>

using namespace std;


std::string getFileContent(std::string file_name)
bool hasOnlyUppercase(const string &str);
bool syntaxIsCorrect(vector<string> first_line);
size_t getFileSize(std::string filename);

# endif