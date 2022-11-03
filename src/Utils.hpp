#ifndef WEBSERV_UTILS_HPP
# define WEBSERV_UTILS_HPP

# include "webserv.hpp"

namespace ft {

    // MEMORY ================================================
    void	    *memcpy(void *dst, const void *src, size_t n);
    void	    bzero(void *s, size_t n);
    // =======================================================

    // STRING ================================================
    size_t      hexToInt(std::string &hex_str);
    int         stoi(const std::string &str);

    std::string toLower(std::string s);
    std::string toUpper(std::string s);
    std::string itoa(int i);
    std::string to_string(int number);

    void        skipWhitespaces(std::string &line, size_t &index);
    void	    trim(std::string &str, const std::string &chars = " \t\n\f\v\r");
    void        trimLeft(std::string &str, const std::string &chars = " \t\n\f\v\r");
    void        trimRight(std::string &str, const std::string &chars = " \t\n\f\v\r");
    void        popBack(std::string &str);
	std::string	getExtension(const std::string &str);
    // =======================================================

    // ARRAY =================================================
    size_t	    strlen(const char *str);
    char	    *strdup(const char *str);
    void	    freeArray(char **array);
    char        **mapToArray(std::map<std::string, std::string> map);
    // =======================================================

    // I/O OPERATIONS ========================================
    bool        isFile(const std::string &path);
	bool		isDirectory(const std::string path);
	std::string	readFile(const std::string &filename);
    int         writeFile(std::string path, std::string body);
    // =======================================================

    // ERRORS / CHECKERS =====================================
    std::string errorMessage(int error);
    std::string craftErrorHTML(int error);
    bool        isOkHTTP(int status);
    // =======================================================

    // =======================================================
    RequestType	RequestFromString(const std::string &str);
    std::string RequestToString(RequestType type);
	std::string timestamp(const std::string &format);
    std::map<std::string, std::string> createMimeMap();
    std::string getMimeExtension(std::map<std::string, std::string> mime_map, const std::string &extension);
    // =======================================================

}

#endif //WEBSERV_UTILS_HPP
