/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-khad <yel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 03:20:18 by yel-khad          #+#    #+#             */
/*   Updated: 2023/03/25 03:28:18 by yel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "method.hpp"

class Post : public Method
{
      public:
        Post(Request request, Server server);
        bool locationSupportUpload();
};