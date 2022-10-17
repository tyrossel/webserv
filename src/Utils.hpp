#ifndef WEBSERV_UTILS_HPP
# define WEBSERV_UTILS_HPP

# include "../includes/webserv.hpp"

namespace ft {
    void	*memcpy(void *dst, const void *src, size_t n);
    void    setupSignals();
    int     stoi(std::string &str);
    void    trimLeft(std::string &str, const std::string &chars = " \t\n\f\v\r");
    void    trimRight(std::string &str, const std::string &chars = " \t\n\f\v\r");
	void	trim(std::string &str, const std::string &chars = " \t\n\f\v\r");
    void    popBack(std::string &str);
    void    skipWhitespaces(std::string &line, size_t &index);
    void	bzero(void *s, size_t n);
    size_t	strlen(const char *str);
    char	*strdup(const char *str);
    void	free_array(char **array);
}

#endif //WEBSERV_UTILS_HPP
