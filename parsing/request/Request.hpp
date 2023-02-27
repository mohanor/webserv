/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 02:24:46 by matef             #+#    #+#             */
/*   Updated: 2023/02/27 04:33:15 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <iostream>
# include "Header.hpp"
# include "../../src/utility/utility.hpp"

#include <sstream>
#include <iostream>

#include <map>
#include <vector>

using namespace std;

class Request
{

    public:
        Request(string method, string resource, string version);
        Request(const Request &copy);
        Request &operator=(const Request &copy);
        ~Request();
        
        string serialize();
        static Request deserialize(const std::string& request);
        static vector<string> getVector(string line, char delimiter = ' ');
    private:
        string _method;
        string _resource;
        string _version;
        // map<string, Header> headers;
};


# endif