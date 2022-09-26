/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trossel <trossel@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 16:47:18 by trossel           #+#    #+#             */
/*   Updated: 2022/09/26 16:53:06 by trossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.hpp"

ConfigParsor::ConfigParsor(const std::string &filename)
	: _filename(filename)
{
}

ConfigParsor::~ConfigParsor()
{
}

ConfigParsor::ConfigParsor(const ConfigParsor &rhs)
{
	*this = rhs;
}

ConfigParsor &ConfigParsor::operator=(const ConfigParsor &rhs)
{
	return (*this);
}

ServerConfig ConfigParsor::parse() const
{
	ServerConfig sc;

	return sc;
}
