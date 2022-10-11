#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../includes/webserv.hpp"
#include "Server.hpp"

class Config {
    private:
        std::vector<Server>         _server;
		bool					    _isValid;

    public:
        // Constructors / Destructor
        Config();
        Config(const Config &other);
        virtual ~Config();

        // Getters
        const std::vector<Server>       getServer() const;
        int                             getNbServer( void ) const;
		bool							isValid() const;
		void							setValid(bool valid);

        // Member functions
        void                            addServer(const Server &newServ);

};

std::ostream &operator<<(std::ostream &out, const Config &rhs);

#endif // CONFIG_HPP
