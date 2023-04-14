/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 14:43:48 by matef             #+#    #+#             */
/*   Updated: 2023/04/14 00:37:21 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <map>

using namespace std;

#define ENDL "\r\n"

class Response
{
    public:
        Response();
        Response(const Response &src);
        Response &operator=(const Response &rhs);
        ~Response();

        void setStatus(int status);
        void setComment(string comment);
        void setHeaders(string key, string value);

        string getHeader();

    private:
        int _status;
        string _comment;
        map<string, string> _headers;

        /*
            size_t pos = body.find(\r\n\r\n) + 4
            
            stringstream ss(body.substr(pos, body.size() - pos));

            while (getline(ss, line, \r\n)
            {
                
            }
            

            
        */
};




# endif