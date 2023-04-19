/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 00:00:00 by yel-khad          #+#    #+#             */
/*   Updated: 2023/04/19 04:47:56 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "method.hpp"
# include "../server/Worker.hpp"



class Get : public Method
{
    public:
        Get(Request request, Server server);
};