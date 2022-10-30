/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trossel <trossel@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 11:11:21 by trossel           #+#    #+#             */
/*   Updated: 2022/10/30 17:26:12 by trossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() :
		max_client_body_size(0),
		isCGI(false),
		auto_index(false),
		path(""),
		root_dir(""),
		cgi_bin(""),
		upload_dir("")
{
	requests_allowed.set();
}

Location::Location(const Location &rhs)
{
	*this = rhs;
}

Location &Location::operator=(const Location &rhs)
{
	if (&rhs == this)
		return *this;
	max_client_body_size = rhs.max_client_body_size;
	requests_allowed = rhs.requests_allowed;
	isCGI = rhs.isCGI;
	indexes = rhs.indexes;
	auto_index = rhs.auto_index;
	path = rhs.path;
	upload_dir = rhs.upload_dir;
	root_dir = rhs.root_dir;
	cgi_bin = rhs.cgi_bin;
	error_pages = rhs.error_pages;
	cgi_extensions = rhs.cgi_extensions;
	redirections = rhs.redirections;
	return *this;
}

Location::~Location()
{
}

const Redirection *	 Location::findRedirection(const std::string &req_path) const
{
	std::map<std::string, Redirection>::const_iterator it = redirections.find(req_path);
	if (it != redirections.end())
		return &(it->second);
	return NULL;
}

std::ostream &operator<<(std::ostream &os, const Location &loc)
{
	std::string method_names[] = {"GET", "POST", "DELETE", "PUT", "HEAD", "PATCH"};

	os << YELLOW << "\ttype: " << RESET;
	if (loc.isCGI)
		os << "CGI PROXY";
	else
	{
		os << "STANDART";
		os << YELLOW << "\n\troot_dir: " << RESET << loc.root_dir;
	}

	os << YELLOW << "\n\tmax_client_body_size: " << RESET << loc.max_client_body_size;

	if (!loc.indexes.empty())
	{
		os << YELLOW << "\n\tindexes: " << RESET;
		for (size_t j = 0; j < loc.indexes.size(); j++)
			os << loc.indexes[j] << " ";
	}

	if (!loc.error_pages.empty())
	{
		os << YELLOW << "\n\terror pages: " << RESET << std::endl;
		for (std::map<int, std::string>::const_iterator it = loc.error_pages.begin();
				it != loc.error_pages.end(); it++)
			os << "\t\t" <<  it->first << ": " << it->second << std::endl;
	}

	if (!loc.redirections.empty())
	{
		os << YELLOW << "\tredirections: " << RESET << std::endl;
		for (std::map<std::string, Redirection>::const_iterator it = loc.redirections.begin();
				it != loc.redirections.end(); it++)
		{
			os << "\t\t" <<  it->first << " --> ";
			if (it->second.new_url.empty())
				os << it->second.status << std::endl;
			else if (it->second.status == 0)
				os << it->second.new_url << " (302)" << std::endl;
			else
				os << it->second.new_url << " (" << it->second.status << ")" << std::endl;
		}
	}

	os << YELLOW << "\tcgi_bin: " << RESET << loc.cgi_bin;
	os << YELLOW << "\n\tcgi_extensions: " << RESET;
	if (!loc.cgi_extensions.empty())
	{
		for (size_t j = 0; j < loc.cgi_extensions.size(); j++)
			os << loc.cgi_extensions[j] << " ";
	}
	else
		os << "none";

	os << YELLOW << "\n\tauto_index: " << RESET << (loc.auto_index ? "true" : "false");

	os << YELLOW << "\n\tRequests disabled: " << RESET;
	if (loc.requests_allowed.all())
		os << "none";
	else
	{
		for (unsigned int i(0); i < Unknown; i++)
		{
			if (!loc.requests_allowed[i])
				os << method_names[i] << " ";
		}
	}
	os << std::endl << std::endl;
	return os;
}
