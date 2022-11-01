# include "Utils.hpp"
#include "webserv.hpp"
#include "sys/stat.h"
#include <ctime>
#include <stdexcept>

int RUNNING;
namespace ft {

    /**************************************************************************************/
    /*                                     MEMORY                                         */
    /**************************************************************************************/

    void	*memcpy(void *dst, const void *src, size_t n)
    {
        size_t i;
        unsigned char *p;
        unsigned char *q;

        i = 0;
        p = (unsigned char *) dst;
        q = (unsigned char *) src;
        while (i < n) {
            p[i] = q[i];
            i++;
        }
        return (dst);
    }

    void	bzero(void *s, size_t n)
    {
        size_t	count;

        count = 0;
        while (count < n)
        {
            ((char *)s)[count] = '\0';
            count++;
        }
    }

    /**************************************************************************************/
    /*                                     STRING                                         */
    /**************************************************************************************/

    size_t  hexToInt(std::string &hex_str)
    {
        if (hex_str.empty())
            return 0;

        std::stringstream stream;
        size_t hex;

        stream << std::hex << hex_str;
        stream >> hex;

        return hex;
    }

    int stoi(std::string &str)
    {
        int i;

        if (str.empty())
            return 0;
        if (str[0] == '+')
            str.erase(0, 1);
        if (std::istringstream(str) >> i)
            return (i);
        else
            throw std::exception();
    }

    std::string toLower(std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }

    std::string toUpper(std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
        return s;
    }

    std::string itoa(int i)
    {
        std::stringstream out;

        out << i;
        return (out.str());
    }

    std::string to_string(int number)
    {
        std::stringstream ss;

        ss << number;
        std::string str = ss.str();

        return (str);
    }

    void skipWhitespaces(std::string &line, size_t &index)
    {
        while (line[index] == ' ' && line[index])
            index += 1;
    }

    void trimLeft(std::string &str, const std::string & chars)
    {
        size_t startpos = str.find_first_not_of(chars);
        if (startpos != std::string::npos)
            str.erase(0, startpos);
    }

    void trimRight(std::string &str, const std::string & chars)
    {
        size_t lastpos = str.find_last_not_of(chars);
        if (lastpos == std::string::npos)
            return;
        str.erase(lastpos + 1, str.length());
    }

	void trim(std::string &str, const std::string &chars)
	{
		trimLeft(str, chars);
		trimRight(str, chars);
	}

    void popBack(std::string &str)
    {
        str.erase(str.size() - 1);
    }

    /**************************************************************************************/
    /*                                     ARRAY                                          */
    /**************************************************************************************/

    size_t strlen(const char *str)
    {
        size_t	count;

        count = 0;
        while (str[count] != '\0')
            count++;
        return (count);
    }

    char *strdup(const char *str)
    {
        int		size_src;
        int		c;
        char	*ret;

        size_src = ft::strlen(str);
        ret = (char *)malloc(sizeof(char) * (size_src + 1));
        if (ret == NULL)
            return (NULL);
        c = 0;
        while (str[c])
        {
            ret[c] = str[c];
            c++;
        }
        ret[c] = '\0';
        return (ret);
    }

    void freeArray(char **array)
    {
        int	i;

        i = 0;
        if (!array)
            return ;
        while (array[i])
            free(array[i++]);
        free(array);
    }

    char **mapToArray(std::map<std::string, std::string> map)
    {
        char **array = NULL;
        int i = 0;

        if (!(array = (char **)malloc(sizeof(char *) * (map.size() + 1))))
            return NULL;

        for (std::map<std::string, std::string>::iterator it = map.begin(); it != map.end(); it++)
        {
            std::string tmp = it->first + "=" + it->second;

            if (!(array[i] = ft::strdup(tmp.c_str())))
                return NULL;
            i++;
        }
        array[i] = NULL;

        return (array);
    }

    /**************************************************************************************/
    /*                                     I/O OPERATIONS                                 */
    /**************************************************************************************/

    bool isFile(const std::string &path)
    {
        struct stat statbuf;
        if (stat(path.c_str(), &statbuf) != 0)
            return false;
        return S_ISREG(statbuf.st_mode);
    }

    bool isDirectory(const std::string path)
	{
		struct stat statbuf;
		if (stat(path.c_str(), &statbuf) != 0)
			return false;
		return S_ISDIR(statbuf.st_mode);
	}

    std::string readFile(const std::string &filename)
    {
        std::string content;
        std::ifstream fs(filename.c_str());
        if (!fs.good())
        {
            throw std::runtime_error("cannot open file: " + filename);
        }
        content.assign(std::istreambuf_iterator<char>(fs),
                       std::istreambuf_iterator<char>());
        fs.close();
        return content;
    }

    /**************************************************************************************/
    /*                                     ERRORS / CHECKERS                              */
    /**************************************************************************************/

    std::string errorMessage(int error)
    {
		std::map<int, std::string> err_map;
        err_map[HTTP_MULTIPLE_CHOICES] = " Multiple choices";
        err_map[HTTP_MOVED_PERMANENTLY] = " Permanent Redirect";
        err_map[HTTP_FOUND] = " Temporary Redirect";
        err_map[HTTP_SEE_OTHER] = " See other";
        err_map[HTTP_NOT_MODIFIED] = " Not Modified";
        err_map[HTTP_TEMPORARY_REDIRECT] = " Temporary Redirect";
        err_map[HTTP_PERMANENT_REDIRECT] = " Permanent Redirect";
        err_map[BAD_REQUEST] = " Bad Request";
        err_map[UNAUTHORIZED] = " Unauthorized";
        err_map[FORBIDDEN] = " Forbidden";
        err_map[NOT_FOUND] = " Not Found";
        err_map[METHOD_NOT_ALLOWED] = " Method Not Allowed";
        err_map[PROXY_AUTHENTICATION] = " Proxy Authentication";
        err_map[REQUEST_TIMEOUT] = " Request Timeout";
        err_map[CONFLICT] = " Conflict";
        err_map[LENGTH_REQUIRED] = " Length Required";
        err_map[PRECONDITION_FAILED] = " Precondition Failed";
        err_map[URI_TOO_LONG] = " URI Too Long";
        err_map[MEDIA_UNSUPPORTED] = " Media Unsupported";
        err_map[RANGE_NOT_SATISFIABLE] = " Range Not Satisfiable";
        err_map[EXPECTATION_FAILED] = " Expectation Failed";
        err_map[TEAPOT] = " I'm a teapot";
        err_map[TOO_MANY_REQUESTS] = " Too Many Requests";
        err_map[UNAVAILABLE_LEGAL_REASON] = " Unvailable Legal Reason";
        err_map[INTERNAL_SERVER_ERROR] = " Internal Server Error";
        err_map[NOT_IMPLEMENTED] = " Not Implemented";
        err_map[BAD_GATEWAY] = " Bad Gateway";
        err_map[SERVICE_UNAVAILABLE] = " Service Unavailable";
        err_map[GATEWAY_TIMEOUT] = " Gateway Timeout";
        err_map[HTTP_VERSION_UNSUPPORTED] = " HTTP Version Unsupported";
        err_map[NOT_EXTENDED] = " Not Extended";
        err_map[NETWORK_AUTHENTICATION_REQUIRED] = " Network Authentication Required";
        err_map[HEADERS_TOO_LARGE] = " Headers Too Large";
        err_map[PAYLOAD_TOO_LARGE] = " Payload Body Size Too Large";

		std::map<int, std::string>::const_iterator error_idx = err_map.find(error);
		if (error_idx != err_map.end())
			return error_idx->second;
		else
            return (" Error");
    }

    std::string craftErrorHTML(int error)
    {
        std::string ret;

        ret.append("<!DOCTYPE html>\n<html lang=\"en\">\n<body>\n<meta charset=\"UTF-8\">\n<h1 style=\"text-align: center\">");
        ret.append(itoa(error));
        ret.append(errorMessage(error));
        ret.append("</h1>\n<hr>\n<body>\n<h2 style=\"text-align: center\">WetServ</h2>\n</body>\n</html>\n");
        return (ret);
    }

    bool isOkHTTP(int status)
    {
        switch (status) {
            case HTTP_OK:
            case HTTP_CREATED:
            case HTTP_ACCEPTED:
            case HTTP_NO_CONTENT:
            case HTTP_RESET_CONTENT:
            case HTTP_PARTIAL_CONTENT:
                return true;
            default:
                return false;
        }
    }

    /**************************************************************************************/
    /*                                    OTHER FUNCTIONS                                 */
    /**************************************************************************************/

    RequestType	RequestFromString(const std::string &str)
    {
        std::map<std::string, RequestType> map;
        map["GET"] = Get;
        map["POST"] = Post;
        map["DELETE"] = Delete;

        try
        {
            return map.at(str);
        }
        catch (const std::exception& e)
        {
            return Unknown;
        }
    }

    std::string RequestToString(RequestType type)
    {
        std::map<RequestType, std::string> map;
        map[Unknown] = "UNKNOWN";
        map[Get] = "GET";
        map[Post] = "POST";
        map[Delete] = "DELETE";

        try
        {
            return map.at(type);
        }
        catch (const std::exception& e)
        {
            return map[Unknown];
        }
    }

    std::string timestamp(const std::string &format)
	{
		const int max_size = 100;
		char tmp[max_size];
		std::string timestamp;
		time_t now = ::time(NULL);
		struct tm *gmtm = localtime(&now);

		std::strftime(tmp, max_size, format.c_str(), gmtm);
		timestamp = tmp;

		return timestamp;
	}
}
