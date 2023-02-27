/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 01:00:16 by matef             #+#    #+#             */
/*   Updated: 2023/02/27 04:35:31 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Header.hpp"

Header::Header(const string &key, const string &value) : key(key), value(value) {}
Header::~Header() {}

string Header::getKey() { return this->key; }
void Header::setValue(const string &value) { this->value = value; }
