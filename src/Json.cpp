#include "Json.hpp"
#include <cctype>

static inline std::string &ltrim(std::string &s)
{
	return s.erase(0, s.find_first_not_of(" \t\n\v\f\r"));
}

static inline std::string &rtrim(std::string &s)
{
	return s.erase(s.find_last_not_of(" \t\n\v\f\r") + 1);
}

bool isValidJsonNumber(const std::string &s)
{
	size_t i(0), minimum(1);
	std::string str = s;
	str = ltrim(rtrim(str));
	if (str[i] == '-')
	{
		i++;
		minimum++;
	}
	while(std::isdigit(str[i]))
		i++;
	if (str[i] == '.' || i < minimum)
		i++;
	while(std::isdigit(str[i]))
		i++;
	if (s[i] && !std::isspace(str[i]))
		return false;
	return true;
	// TODO TYR: handle scientific power
}

bool isValidJsonString(const std::string &s)
{
	int i = 0;
	std::string str = s;
	str = ltrim(rtrim(str));
	const std::string ref("\"\\/bfnrt");
	if (s.size() == 0 || s[0] != '"' || s[s.size() - 1] != '"')
		return false;
	if(str.at(i) == '"')
		i++;
	while(str[i] != '"' && i < s.size())
	{
		if (str[i] < 32 || str[i] > 126)
			return false;
		// TODO TYR: handle "\uABCD"
		if (str[i] == '\\')
		{
			if (ref.find_first_of(str[i+1]) == std::string::npos)
				return false;
			i += 2;
			continue ;
		}
		i++;
	}
	return true;
}

bool isValidJsonConstant(const std::string &s)
{
	if (s == "true" || s == "false" || s == "null")
		return true;
	return false;
}

bool isValidJsonArray(const std::string &s)
{
	std::string str = s;
	str = ltrim(rtrim(str));
	return false;
}

// bool isValidJsonObject(std::string s)
// {
// 	if (s.size() == 0)
// 	{
// 		std::cerr << "Empty string" << std::endl;
// 		return false;
// 	}
// 	s = ltrim(rtrim(s));
// 	if (s.size() == 0 || s[0] != '{' || s[s.size() - 1] != '}')
// 	{
// 		std::cerr << "No JSON object" << std::endl;
// 		return false;
// 	}
// 	s = s.substr(1, s.size() - 3);
// 	std::cout << "object = " << s << std::endl;
// 	std::string key, value;
// 	size_t end_of_key;
// 	do
// 	{
// 		s = ltrim(s);
// 		if(s[0] != '"')
// 		{
// 			std::cerr << "Key doesnt start with quote:" << s << std::endl;
// 			return false;
// 		}
// 		end_of_key = s.find_first_of('"', 1);
// 		if (end_of_key == std::string::npos)
// 		{
// 			std::cerr << "No end of key" << std::endl;
// 			return false;
// 		}
// 		key = s.substr(1, end_of_key - 1);
// 		s = s.substr(end_of_key + 1);
// 		s = ltrim(s);
// 		if (s[0] != ':')
// 		{
// 			std::cerr << "No colon" << std::endl;
// 			return false;
// 		}
// 		s = s.substr(1);
// 		s = ltrim(s);
// 		if(s[0] == '{')
// 		{
// 			size_t end_of_object = s.find_first_of("adsf");
// 		}
// 		else if ('[') {}
//
// 		else if (s[0] == '"') {}
// 		else if (std::isdigit(s[0]) || s[0] == '-') {}
// 		else
// 			return false;
//
//
// 		std::cout << "key = '" << key << "'" << std::endl;
//
//
// 	}
// 	//while (false);
// 	while(!key.empty());
// 	return true;
//
// }
