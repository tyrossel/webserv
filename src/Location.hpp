/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trossel <trossel@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 11:08:24 by trossel           #+#    #+#             */
/*   Updated: 2022/10/21 11:47:25 by trossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_LOCATION_HPP
#define WEBSERV_LOCATION_HPP

#include "../includes/webserv.hpp"

struct Location
{
	Location();

	void disableRequest(RequestType type);
	bool isRequestAllowed(RequestType type) const;

	int		max_client_body_size;
	int		requests_allowed;
	bool	isCGI;
	std::string	root_dir;
	std::string	cgi_path;
};

std::ostream &operator<<(std::ostream &os, const Location &loc);


#endif // WEBSERV_LOCATION_HPP
