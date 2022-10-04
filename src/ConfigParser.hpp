/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trossel <trossel@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 16:37:06 by trossel           #+#    #+#             */
/*   Updated: 2022/10/04 14:06:04 by trossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

#include "Config.hpp"

class ConfigParsor
{

	// 	class FileNotFoundException : public std::exception{
	// 	const std::string _msg;
	// 	public:
	// 		FileNotFoundException(const ConfigParsor &c) : _msg(c.getFilename() + " cannot be read."){}
	// 		~FileNotFoundException() throw() {}
	// 		virtual const char * what() const throw ()
	// 	{
	// 		return (_msg.c_str());
	// 	}
	// };

	public:

		ConfigParsor(const std::string &filename);
		ConfigParsor(const ConfigParsor &);
		ConfigParsor &operator=(const ConfigParsor &);
		~ConfigParsor();


		const std::string &getFilename() const;

		Config parse() const;

	private:

		std::string	_filename;

};
