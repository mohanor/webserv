/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelhaim <yoelhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 21:56:55 by yoelhaim          #+#    #+#             */
/*   Updated: 2023/03/24 16:01:41 by yoelhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <dirent.h>
#include  <errno.h>
#include "method.hpp"
#include "../../parsing/request/Request.hpp"
#include "../../parsing/conf/Server.hpp"


class Delete : public Method
{
private:
    /* data */
public:
    Delete(Request request, Server server);
    ~Delete();
    void getResourceType();
    void deleteResource();
    void MissingBackSlash();
    void deleteAllFolderContent();
    void deleteSeccess();
    void setError(int status, string comment);
    
};

