#ifndef WEBSERV_UTILS_HPP
# define WEBSERV_UTILS_HPP

# include "../includes/webserv.hpp"

namespace ft {

    void	    *memcpy(void *dst, const void *src, size_t n);
    void	    bzero(void *s, size_t n);

    std::string toLower(std::string s);
    std::string toUpper(std::string s);

    size_t      hexToInt(std::string &hex_str);
    int         stoi(std::string &str);
    std::string to_string(int number);
    void        skipWhitespaces(std::string &line, size_t &index);
    void	    trim(std::string &str, const std::string &chars = " \t\n\f\v\r");
    void        trimLeft(std::string &str, const std::string &chars = " \t\n\f\v\r");
    void        trimRight(std::string &str, const std::string &chars = " \t\n\f\v\r");
    void        popBack(std::string &str);

    void        setupSignals();

    size_t	    strlen(const char *str);
    char	    *strdup(const char *str);
    void	    freeArray(char **array);
    char        **mapToArray(std::map<std::string, std::string> map);

    RequestType	RequestFromString(const std::string &str);
	bool		isDirectory(const std::string path);

    std::string itoa(int i);
    std::string errorMessage(int error);
    std::string craftErrorHTML(int error);
    bool        isOkHTTP(int status);
    bool        isFile(const std::string &path);
}

#endif //WEBSERV_UTILS_HPP
