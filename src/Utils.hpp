#ifndef WEBSERV_UTILS_HPP
# define WEBSERV_UTILS_HPP

# include "../includes/webserv.hpp"

namespace ft {
    int     stoi(std::string &str);
    void    trimLeft(std::string &str, char c);
    void    trimRight(std::string &str, char c);
    void    popBack(std::string &str);
    void    skipWhitespaces(std::string &line, size_t &index);
    void	bzero(void *s, size_t n);
}

#endif //WEBSERV_UTILS_HPP