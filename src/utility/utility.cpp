/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 21:42:58 by matef             #+#    #+#             */
/*   Updated: 2023/02/23 22:31:21 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utility.hpp"

std::string get_file_content(std::string file_name)
{
    std::string content, line;
    std::ifstream MyReadFile(file_name);

    if (!MyReadFile.is_open())
        return "";
    int i = 0;
    while (getline (MyReadFile, line))
    {
        std::cout << i++ << " " << line << std::endl;
        content += line + '\n';
    }

    MyReadFile.close();
    return content.substr(0, content.size() - 1);
}


