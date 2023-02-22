/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 16:06:34 by matef             #+#    #+#             */
/*   Updated: 2023/02/21 16:47:28 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Webserv::Webserv()
{
}

Webserv::~Webserv()
{
}

Webserv::Webserv(Webserv const &src)
{
    *this = src;
}

Webserv &Webserv::operator=(Webserv const &rhs)
{
    if (this != &rhs)
    {
    }
    return *this;
}