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

    int stoi(const std::string &s)
    {
		std::string str = s;
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

	std::string getExtension(const std::string &path)
	{
		size_t dot = path.find_last_of('.');
		if (dot == path.npos || dot == path.size() - 1)
			return "";
		return path.substr(dot + 1);
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

    int writeFile(std::string path, std::string body)
    {
        std::string content;
        std::ofstream ofs(path.c_str(), std::ofstream::app);
        if (!ofs.good())
        {
            return (-1);
        }
        ofs << body.c_str() << " || ";
        ofs.close();
        return 0;
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
        ret.append(ft::itoa(error));
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

    std::map<std::string, std::string> createMimeMap()
    {
        std::map<std::string, std::string> mime_map;

        mime_map["html"] = "text/html";
        mime_map["htm"] = "text/html";
        mime_map["shtml"] = "text/html";
        mime_map["css"] = "text/css";
        mime_map["xml"] = "text/xml";
        mime_map["gif"] = "image/gif";
        mime_map["jpeg"] = "image/jpeg";
        mime_map["jpg"] = "image/jpeg";
        mime_map["js"] = "application/javascript";
        mime_map["atom"] = "application/atom+xml";
        mime_map["rss"] = "application/rss+xml";

        mime_map["mml"] = "text/mathml";
        mime_map["txt"] = "text/plain";
        mime_map["jad"] = "text/vnd.sun.j2me.app-descriptor";
        mime_map["wml"] = "text/vnd.wap.wml";
        mime_map["htc"] = "text/x-component";

        mime_map["png"] = "image/png";
        mime_map["svg"] = "image/svg+xml";
        mime_map["svgz"] = "image/svg+xml";
        mime_map["tif"] = "image/tiff";
        mime_map["tiff"] = "image/tiff";
        mime_map["wbmp"] = "image/vnd.wap.wbmp";
        mime_map["webp"] = "image/webp";
        mime_map["ico"] = "image/x-icon";
        mime_map["jng"] = "image/x-jng";
        mime_map["bmp"] = "image/x-ms-bmp";

        mime_map["woff"] = "font/woff";
        mime_map["woff2"] = "font/woff2";

        mime_map["jar"] = "application/java-archive";
        mime_map["war"] = "application/java-archive";
        mime_map["ear"] = "application/java-archive";
        mime_map["json"] = "application/json";
        mime_map["hqx"] = "application/mac-binhex40";
        mime_map["doc"] = "application/msword";
        mime_map["pdf"] = "application/pdf";
        mime_map["ps"] = "application/postscript";
        mime_map["eps"] = "application/postscript";
        mime_map["ai"] = "application/postscript";
        mime_map["rtf"] = "application/rtf";
        mime_map["m3u8"] = "application/vnd.apple.mpegurl";
        mime_map["kml"] = "application/vnd.google-earth.kml+xml";
        mime_map["kmz"] = "application/vnd.google-earth.kmz";
        mime_map["xls"] = "application/vnd.ms-excel";
        mime_map["eot"] = "application/vnd.ms-fontobject";
        mime_map["ppt"] = "application/vnd.ms-powerpoint";
        mime_map["odg"] = "application/vnd.oasis.opendocument.graphics";
        mime_map["odp"] = "application/vnd.oasis.opendocument.presentation";
        mime_map["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
        mime_map["odt"] = "application/vnd.oasis.opendocument.text";

        mime_map["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
        mime_map["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
        mime_map["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";

        mime_map["wmlc"] = "application/vnd.wap.wmlc";
        mime_map["7z"] = "application/x-7z-compressed";
        mime_map["cco"] = "application/x-cocoa";
        mime_map["jardiff"] = "application/x-java-archive-diff";
        mime_map["jnlp"] = "application/x-java-jnlp-file";
        mime_map["run"] = "application/x-makeself";
        mime_map["pl"] = "application/x-perl";
        mime_map["pm"] = "application/x-perl";
        mime_map["prc"] = "application/x-pilot";
        mime_map["pdb"] = "application/x-pilot";
        mime_map["rar"] = "application/x-rar-compressed";
        mime_map["rpm"] = "application/x-redhat-package-manager";
        mime_map["sea"] = "application/x-sea";
        mime_map["swf"] = "application/x-shockwave-flash";
        mime_map["sit"] = "application/x-stuffit";
        mime_map["tcl"] = "application/x-tcl";
        mime_map["tk"] = "application/x-tcl";
        mime_map["der"] = "application/x-x509-ca-cert";
        mime_map["pem"] = "application/x-x509-ca-cert";
        mime_map["crt"] = "application/x-x509-ca-cert";
        mime_map["xpi"] = "application/x-xpinstall";
        mime_map["xhtml"] = "application/xhtml+xml";
        mime_map["xspf"] = "application/xspf+xml";
        mime_map["zip"] = "application/zip";

        mime_map["bin"] = "application/octet-stream";
        mime_map["exe"] = "application/octet-stream";
        mime_map["dll"] = "application/octet-stream";
        mime_map["deb"] = "application/octet-stream";
        mime_map["dmg"] = "application/octet-stream";
        mime_map["iso"] = "application/octet-stream";
        mime_map["img"] = "application/octet-stream";
        mime_map["msi"] = "application/octet-stream";
        mime_map["msp"] = "application/octet-stream";
        mime_map["msm"] = "application/octet-stream";

        mime_map["mid"] = "audio/midi";
        mime_map["midi"] = "audio/midi";
        mime_map["kar"] = "audio/midi";
        mime_map["mp3"] = "audio/mpeg";
        mime_map["ogg"] = "audio/ogg";
        mime_map["m4a"] = "audio/x-m4a";
        mime_map["ra"] = "audio/x-realaudio";

        mime_map["3gpp"] = "video/3gpp";
        mime_map["3gp"] = "video/3gpp";
        mime_map["ts"] = "video/mp2t";
        mime_map["mp4"] = "video/mp4";
        mime_map["mpeg"] = "video/mpeg";
        mime_map["mpg"] = "video/mpeg";
        mime_map["mov"] = "video/quicktime";
        mime_map["webm"] = "video/webm";
        mime_map["flv"] = "video/x-flv";
        mime_map["m4v"] = "video/x-m4v";
        mime_map["mng"] = "video/x-mng";
        mime_map["asx"] = "video/x-ms-asf";
        mime_map["asf"] = "video/x-ms-asf";
        mime_map["wmv"] = "video/x-ms-wmv";
        mime_map["avi"] = "video/x-msvideo";

        return mime_map;
    }

    std::string getMimeExtension(std::map<std::string, std::string> mime_map, const std::string &extension)
    {
        if (!extension.length())
            return "application/octet-stream";
        if (mime_map.count(extension))
            return mime_map[extension];
        return "application/octet-stream";
    }
}
