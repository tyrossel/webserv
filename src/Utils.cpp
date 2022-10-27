# include "Utils.hpp"
#include "webserv.hpp"
#include "sys/stat.h"

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

//    unsigned int strToIp(std::string str)
//    {
//        char c_str[3];
//        int n = 0;
//
//        for (int i = 0; i < str.size(); i++) {
//            i += digitToDot(str.c_str(), i + 1);
//            c_str[n++] = substr(x, x, );
//        }
//    }
//
//    int digitToDot(char *str, int occurence)
//    {
//        int ret = (occurence * -1) + 1;
//        for (int i = 0; str[i] != '.' && occurence; i++)
//        {
//            if (str[i] == '.')
//                occurence--;
//        }
//        ret = i;
//        return (ret);
//    }


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

	void	trim(std::string &str, const std::string &chars)
	{
		trimLeft(str, chars);
		trimRight(str, chars);
	}

    void popBack(std::string &str)
    {
        str.erase(str.size() - 1);
    }


    /**************************************************************************************/
    /*                                     SIGNALS                                        */
    /**************************************************************************************/

    void stopLoop(int sig)
    {
        RUNNING = 0;
        std::cout << RED << "Signal " << sig << " catched. Loop is being stopped." << RESET << std::endl;
    }

    void setupSignals()
    {
        RUNNING = 1;
        signal(SIGINT, stopLoop);
    }


    /**************************************************************************************/
    /*                                     ARRAY                                          */
    /**************************************************************************************/

    size_t	strlen(const char *str)
    {
        size_t	count;

        count = 0;
        while (str[count] != '\0')
            count++;
        return (count);
    }

    char	*strdup(const char *str)
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

    void	freeArray(char **array)
    {
        int	i;

        i = 0;
        if (!array)
            return ;
        while (array[i])
            free(array[i++]);
        free(array);
    }

    char        **mapToArray(std::map<std::string, std::string> map)
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

    RequestType	RequestFromString(const std::string &str)
    {
		std::map<std::string, RequestType> map;
			map["GET"] = Get;
			map["POST"] = Post;
			map["DELETE"] = Delete;
			map["HEAD"] = Head;
			map["PATCH"] = Patch;
			map["PUT"] = Put;

		try
		{
			return map.at(str);
		}
		catch (const std::exception& e)
		{
			return Unknown;
		}

	}

	bool isDirectory(const std::string path)
	{
		struct stat statbuf;
		if (stat(path.c_str(), &statbuf) != 0)
			return false;
		return S_ISDIR(statbuf.st_mode);
	}

    std::string itoa(int i)
    {
        std::stringstream out;

        out << i;
        return (out.str());
    }

    std::string errorMessage(int error)
    {
        if (error == BAD_REQUEST)
            return (" Bad Request");
        else if (error == UNAUTHORIZED)
            return (" Unauthorized");
        else if (error == FORBIDDEN)
            return (" Forbidden");
        else if (error == NOT_FOUND)
            return (" Not Found");
        else if (error == METHOD_NOT_ALLOWED)
            return (" Method Not Allowed");
        else if (error == PROXY_AUTHENTICATION)
            return (" Proxy Authentication");
        else if (error == REQUEST_TIMEOUT)
            return (" Request Timeout");
        else if (error == CONFLICT)
            return (" Conflict");
        else if (error == LENGTH_REQUIRED)
            return (" Length Required");
        else if (error == PRECONDITION_FAILED)
            return (" Precondition Failed");
        else if (error == URI_TOO_LONG)
            return (" URI Too Long");
        else if (error == MEDIA_UNSUPPORTED)
            return (" Media Unsupported");
        else if (error == RANGE_NOT_SATISFIABLE)
            return (" Range Not Satisfiable");
        else if (error == EXPECTATION_FAILED)
            return (" Expectation Failed");
        else if (error == TEAPOT)
            return (" I'm a teapot");
        else if (error == TOO_MANY_REQUESTS)
            return (" Too Many Requests");
        else if (error == UNAVAILABLE_LEGAL_REASON)
            return (" Unvailable Legal Reason");
        else if (error == INTERNAL_SERVER_ERROR)
            return (" Internal Server Error");
        else if (error == NOT_IMPLEMENTED)
            return (" Not Implemented");
        else if (error == BAD_GATEWAY)
            return (" Bad Gateway");
        else if (error == SERVICE_UNAVAILABLE)
            return (" Service Unavailable");
        else if (error == GATEWAY_TIMEOUT)
            return (" Gateway Timeout");
        else if (error == HTTP_VERSION_UNSUPPORTED)
            return (" HTTP Version Unsupported");
        else if (error == NOT_EXTENDED)
            return (" Not Extended");
        else if (error == NETWORK_AUTHENTICATION_REQUIRED)
            return (" Network Authentication Required");
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
                return true;
            case HTTP_CREATED:
                return true;
            case HTTP_ACCEPTED:
                return true;
            case HTTP_NO_CONTENT:
                return true;
            case HTTP_RESET_CONTENT:
                return true;
            case HTTP_PARTIAL_CONTENT:
                return true;
            default:
                return false;
        }
    }

    bool isFile(const std::string &path)
    {
        struct stat statbuf;
        if (stat(path.c_str(), &statbuf) != 0)
            return false;
        return S_ISREG(statbuf.st_mode);
    }
}
