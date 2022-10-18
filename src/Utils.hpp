#ifndef WEBSERV_UTILS_HPP
# define WEBSERV_UTILS_HPP

# include "../includes/webserv.hpp"

namespace ft {

    void	    *memcpy(void *dst, const void *src, size_t n);
    void	    bzero(void *s, size_t n);

    int         stoi(std::string &str);
    std::string to_string(int number);
    void        skipWhitespaces(std::string &line, size_t &index);
    void        trimLeft(std::string &str, const std::string &chars = " \t\n\f\v\r");
    void        trimRight(std::string &str, const std::string &chars = " \t\n\f\v\r");
    void        popBack(std::string &str);

    void        setupSignals();

    size_t	    strlen(const char *str);
    char	    *strdup(const char *str);
    void	free_array(char **array);
}

#endif //WEBSERV_UTILS_HPP
