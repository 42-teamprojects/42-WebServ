
#include "webserv.hpp"

template <typename T>
std::string toString(T value)
{
    std::ostringstream os;
    os << value;
    return os.str();
}

template <typename Container>
Container ft_split(const std::string &s, const std::string &delimiters)
{
    Container splited;
    size_t i = 0;

    while (i < s.length())
    {
        i = s.find_first_not_of(delimiters, i);
        
        if (i == std::string::npos)
            break;
        size_t wordEnd = s.find_first_of(delimiters, i);
        
        splited.push_back(s.substr(i, wordEnd - i));
        
        i = wordEnd;
    }
    return splited;
}