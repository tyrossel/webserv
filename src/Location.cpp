/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trossel <trossel@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 11:11:21 by trossel           #+#    #+#             */
/*   Updated: 2022/10/21 13:52:23 by trossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() :
		max_client_body_size(0),
		requests_allowed(0xFF),
		isCGI(false),
		root_dir(""),
		cgi_bin("")
{
}

void Location::disableRequest(RequestType type)
{
	requests_allowed &= ~(1UL << type);
}

bool Location::isRequestAllowed(RequestType type) const
{
	return ((requests_allowed >> type) & 1U);
}

std::ostream &operator<<(std::ostream &os, const Location &loc)
{
	std::string method_names[] = {"GET", "POST", "DELETE", "PUT", "HEAD", "PATCH"};
	os << "\tmax_client_body_size : " << loc.max_client_body_size;

	if (loc.isCGI)
		os << "\n\tcgi_bin: " << loc.cgi_bin;
	else
		os << "\n\tRoot dir:" << loc.root_dir;

	if (!loc.indexes.empty())
	{
		os << "\n\tIndexes: ";
		for (size_t j = 0; j < loc.indexes.size(); j++)
			os << loc.indexes[j] << " ";
	}

	if (!loc.cgi_extensions.empty())
	{
		os << "\n\tCGI extensions: ";
		for (size_t j = 0; j < loc.cgi_extensions.size(); j++)
			os << loc.cgi_extensions[j] << " ";
	}

	if (loc.requests_allowed < 0xFF)
	{
		os << "\n\tRequests disabled : ";
		for (unsigned int i(1); i < 7; i++)
		{
			if (!loc.isRequestAllowed((RequestType)i))
				os << method_names[i - 1] << " ";
		}
	}
	os << std::endl;
	return os;
}
