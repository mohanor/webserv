/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 01:00:16 by matef             #+#    #+#             */
/*   Updated: 2023/02/28 00:06:51 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Header.hpp"

Header::Header(const string &key, const string &value) : key(key), value(value) {}
Header::~Header() {}

string Header::getKey() { return this->key; }
string Header::getValue() { return this->value; }
void Header::setValue(const string &value) { this->value = value; }
