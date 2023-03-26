/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-khad <yel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 00:00:00 by yel-khad          #+#    #+#             */
/*   Updated: 2023/03/26 02:15:48 by yel-khad         ###   ########.fr       */
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