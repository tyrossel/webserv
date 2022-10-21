/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trossel <trossel@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 11:11:21 by trossel           #+#    #+#             */
/*   Updated: 2022/10/21 12:00:00 by trossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() :
		max_client_body_size(0),
		requests_allowed(0xFF),
		isCGI(false),
		root_dir(""),
		cgi_path("")
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
	os << "\tmax_client_body_size : " << loc.max_client_body_size << std::endl;
	if (loc.isCGI)
		os << "\tCGI bin: " << loc.cgi_path << std::endl;
	else
		os << "\tRoot dir:" << loc.root_dir << std::endl;
	os << "\tRequests allowed : ";
	if (loc.requests_allowed < 0xFF)
	{
		for (unsigned int i(1); i < 7; i++)
		{
			if (loc.isRequestAllowed((RequestType)i))
				os << method_names[i - 1] << ", ";
		}
	}
	else
		os << "all";
	os << std::endl;
	return os;
}
