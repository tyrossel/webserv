/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trossel <trossel@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 11:08:24 by trossel           #+#    #+#             */
/*   Updated: 2022/10/30 12:59:46 by trossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_LOCATION_HPP
#define WEBSERV_LOCATION_HPP

#include "webserv.hpp"

struct Redirection
{
	std::string	new_url;
	int			status;
};

struct Location
{
	Location();
	Location(const Location &rhs);
	Location &operator=(const Location &rhs);
	~Location();

	const Redirection *	 findRedirection(const std::string &req_path) const;

	long 								max_client_body_size;
	std::bitset<LastRequestType>		requests_allowed;
	bool								isCGI;
	bool								auto_index;
	std::string							path;				// In the client request
	std::string							root_dir;			// Where to search for file
	std::string							cgi_bin;
	std::vector<std::string>			cgi_extensions;
	std::string							upload_dir;
	std::vector<std::string>			indexes;
	std::map<int, std::string>			error_pages;
	std::map<std::string, Redirection>	redirections;
};

std::ostream &operator<<(std::ostream &os, const Location &loc);

#endif // WEBSERV_LOCATION_HPP
