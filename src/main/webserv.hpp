/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 16:02:12 by matef             #+#    #+#             */
/*   Updated: 2023/02/21 16:46:42 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>

class Webserv
{
    public:
        Webserv();
        ~Webserv();
        Webserv(Webserv const &src);
        Webserv &operator=(Webserv const &rhs);
};


# endif