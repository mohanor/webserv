/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 05:05:57 by matef             #+#    #+#             */
/*   Updated: 2023/03/18 23:19:33 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIMETYPES_HPP
# define MIMETYPES_HPP

# include <iostream>
# include <fstream>

# include <map>

#include "../request/Request.hpp"

using namespace std;

class MimeTypes
{
    public:
        MimeTypes();
        ~MimeTypes();
        MimeTypes &operator=(const MimeTypes &rhs);
        MimeTypes(const MimeTypes &rhs);

        string getMimeType(string extension);
        void parseMimeTypes(string file);
        bool isSyntaxValid(string file);
        
        bool isSemicolonInLast(string extension);
        bool allowedChars(string line);

        map<string, string> getMimeTypes() const;

    private:
        map<string, string> mimeTypes;
        int lineCounter;
};


#endif