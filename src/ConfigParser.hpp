/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trossel <trossel@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 16:37:06 by trossel           #+#    #+#             */
/*   Updated: 2022/10/21 14:38:31 by trossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

#include "Config.hpp"
#include "Json.hpp"

class ConfigParsor
{

	public:

		ConfigParsor(const std::string &filename);
		ConfigParsor(const ConfigParsor &);
		ConfigParsor &operator=(const ConfigParsor &);
		~ConfigParsor();


		const std::string &getFilename() const;

		Config parse() const;

	private:

		Location parseLocation(const JsonObject &locObject, const Location &defaultLoc) const;
		Server parseServer(const JsonObject &serverObject) const;

		std::string	_filename;

};
