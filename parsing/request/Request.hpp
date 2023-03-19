/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 02:24:46 by matef             #+#    #+#             */
/*   Updated: 2023/03/09 01:35:42 by matef            ###   ########.fr       */
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
#include <deque>

#define URI_MAX_SIZE 2048
#define ALLOWED_CHARACTERS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;="


// status of request

#define OK 200
#define BAD_REQUEST 400
#define NOT_FOUND 404
#define METHOD_NOT_ALLOWED 405
#define REQUEST_URI_TOO_LONG 414
#define INTERNAL_SERVER_ERROR 500
#define NOT_IMPLEMENTED 501
#define VERSION_NOT_SUPPORTED 505


using namespace std;

class Request
{
    public:
        typedef map<string, Header> Headers;

    public:
        Request(string method, string resource, string version, Headers headers = Headers());
        Request(const Request &copy);
        Request &operator=(const Request &copy);
        ~Request();

        string serialize();
        static Request deserialize(const string& request);
        static vector<string> getVector(string line, char delimiter = ' ');
        
		void resource();
		bool syntaxError();
		string getRessource();
        int isReqWellFormed();
        bool isMethodAllowed();
        bool allowedChars();
        // bool transferEncoding();
        bool acceptUriLength();
        bool isVersionSupported();

    private:
        string _method;
        string _resource;
        string _version;
        map<string, Header> _headers;
};


# endif