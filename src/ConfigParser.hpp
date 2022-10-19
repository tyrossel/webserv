/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trossel <trossel@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 16:37:06 by trossel           #+#    #+#             */
/*   Updated: 2022/10/19 16:38:46 by trossel          ###   ########.fr       */
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

		Server parseServer(const JsonObject &serverObject) const;
		Config parse() const;

	private:

		std::string	_filename;

};
