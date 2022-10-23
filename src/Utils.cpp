# include "Utils.hpp"
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
}


