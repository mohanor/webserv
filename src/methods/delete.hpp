/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-khad <yel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 02:21:20 by yel-khad          #+#    #+#             */
/*   Updated: 2023/04/05 02:21:41 by yel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <dirent.h>
#include <errno.h>
#include <cstdio>
#include "method.hpp"
#include "../../parsing/request/Request.hpp"
#include "../../parsing/conf/Server.hpp"


class Delete : public Method
{
    public:
        Delete(Request request, Server server);
};

