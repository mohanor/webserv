/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 00:00:00 by yel-khad          #+#    #+#             */
/*   Updated: 2023/04/13 16:50:44 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "method.hpp"
# include "../server/Worker.hpp"

extern MimeTypes _mime;

class Get : public Method
{
    public:
        Get(Request request, Server server);
};