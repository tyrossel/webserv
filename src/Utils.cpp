# include "Utils.hpp"
int RUNNING;
namespace ft {

    void stopLoop(int sig)
    {
        RUNNING = 0;
        (void)sig;
    }

    void setupSignals()
    {
        RUNNING = 1;
        signal(SIGINT, stopLoop);
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

    void trimLeft(std::string &str, char c)
    {
        size_t startpos = str.find_first_not_of(c);
        if (startpos != std::string::npos)
            str.erase(0, startpos);
    }

    void trimRight(std::string &str, char c)
    {
        size_t lastpos = str.find_last_not_of(c);
        if (lastpos == std::string::npos)
            return;
        str.erase(lastpos + 1, str.length());
    }

    void popBack(std::string &str)
    {
        str.erase(str.size() - 1);
    }

    void skipWhitespaces(std::string &line, size_t &index)
    {
        while (line[index] == ' ' && line[index])
            index += 1;
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
}
