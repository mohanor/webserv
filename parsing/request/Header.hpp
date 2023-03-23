/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-khad <yel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 01:02:48 by matef             #+#    #+#             */
/*   Updated: 2023/03/22 14:29:24 by yel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_HPP
#define HEADER_HPP

#include <iostream>

using namespace std;

class Header
{

    public:
        Header(const string &key, const string &value);
        ~Header();

        string getKey();
        void setValue(const string &value);
        string getValue();

    private:
        string key;
        string value;

};

#endif



/*
class Request
{
    private:
        Version version;
        Method method;
        std::string resource;
        std::map<std::string, Header> headers;

    public:
        static Request deserialize(const std::string& request)
        {
            std::vector<std::string> lines = split(request, std::string(LINE_END));

            if (lines.size() < 1)
            {
                throw std::runtime_error("HTTP Request ('" + std::string(request) + "') consisted of " + std::to_string(lines.size()) + " lines, should be >= 1.");
            }

            std::vector<std::string> segments = split(lines[0], " ");

            if (segments.size() != 3)
            {
                throw std::runtime_error("First line of HTTP request ('" + std::string(request) + "') consisted of " + std::to_string(segments.size()) + " space separated segments, should be 3.");
            }

            const Method method = method_from_string(segments[0]);
            const std::string resource = segments[1];
            const Version version = version_from_string(segments[2]);

            std::map<std::string, Header> headers;

            for (std::size_t i = 1; i < lines.size(); i++)
            {
                if (lines[i].size() > 0)
                {
                    const Header header = Header::deserialize(lines[i]);
                    headers.insert(std::make_pair(header.get_key(), header));
                }
            }

            return Request(method, resource, headers, version);
        }
};
*/