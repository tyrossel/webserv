#include "Json.hpp"
#include <cctype>
#include <stdexcept>
#include <sstream>

static inline std::string &ltrim(std::string &s)
{
	return s.erase(0, s.find_first_not_of(" \t\n\v\f\r"));
}

static inline std::string &rtrim(std::string &s)
{
	return s.erase(s.find_last_not_of(" \t\n\v\f\r") + 1);
}

bool isValidJsonValue(const std::string &s, bool checkStartOnly)
{
	std::string str = s;
	str = ltrim(rtrim(str));
	if (str[0] == '"')
		return isValidJsonString(str, checkStartOnly);
	else if (str[0] == '-' || std::isdigit(str[0]))
		return isValidJsonNumber(str, checkStartOnly);
	else if (str[0] == '[')
		return isValidJsonArray(str, checkStartOnly);
	else if (str[0] == '{')
		return isValidJsonObject(str, checkStartOnly);
	else
		return isValidJsonConstant(str, checkStartOnly);
}

bool isValJsonNumber(const std::string &s, bool checkStartOnly)
{
	size_t i(0);
	std::string str = s;
	str = ltrim(rtrim(str));
	if (str[i] == '-')
		i++;
	if (!std::isdigit(str[i]))
		return false;
	while(std::isdigit(str[i]))
		i++;
	if (str[i] == '.')
	{
		if (!std::isdigit(str[i+1]))
			return false;
		i++;
		while(std::isdigit(str[i]))
			i++;
	}
	// TODO TYR: handle scientific power
	if (!checkStartOnly && i != str.size())
		return false;
	return true;
}

bool isValidJsonString(const std::string &s, bool checkStartOnly)
{
	int i = 0;
	std::string str = s;
	str = ltrim(rtrim(str));
	const std::string escaped_chars("\"\\/bfnrt");
	size_t last_quote = 0;
	if (s.size() == 0 || s[0] != '"')
		return false;
	do {
		last_quote = str.find_first_of('"', last_quote + 1);
	} while (last_quote != std::string::npos && str[last_quote - 1] == '\\');
	if (last_quote == std::string::npos)
		return false;
	if(str.at(i) == '"')
		i++;
	while(i < s.size() && str[i] != '"')
	{
		if (str[i] < 32 || str[i] > 126)
			return false;
		// TODO TYR: handle "\uABCD"
		if (str[i] == '\\')
		{
			if (escaped_chars.find_first_of(str[i+1]) == std::string::npos)
				return false;
			i++;
		}
		i++;
	}
	if (!checkStartOnly && i != str.size() - 1)
		return false;
	return true;
}

bool isValidJsonConstant(const std::string &s, bool checkStartOnly)
{
	if (checkStartOnly)
	{
		if (s == "true" || s == "false" || s == "null")
			return true;
	}
	else
	{
		if (s.substr(0, 4) == "true" || s.substr(0, 5) == "false" || s.substr(0, 4) == "null")
			return true;
	}
	return false;
}

bool isValidJsonArray(const std::string &s, bool checkStartOnly)
{
	std::string str = s;
	size_t i(0), valueSize(0);
	str = ltrim(rtrim(str));
	if (s.size() == 0 || s[0] != '[')
		return false;
	str = str.substr(1);
	str = ltrim(rtrim(str));
	// do
	// {
	// 	if (!isValidJsonValue(str, true))
	// 		return false;
	// 	str = str.substr(valueSize + 1);
	// 	str = ltrim(rtrim(str));
	//
	// } while();
	return false;
}

bool isValidJsonObject(std::string s, bool checkStartOnly)
{
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
	return true;
//
}

std::string	JsonObject::parseJsonKey(std::string &s)
{
	size_t end_of_key;
	std::string key("");

	s = ltrim(s);
	if(s[0] != '"')
		throw std::logic_error("Key doesnt start with quote:");
	end_of_key = s.find_first_of('"', 1);
	if (end_of_key == std::string::npos)
		throw std::logic_error("No end of key");
	key = s.substr(1, end_of_key - 1);
	s = s.substr(end_of_key + 1);
	s = ltrim(s);
	if (s[0] != ':')
		throw std::logic_error("JSON error: no colon after key '" + key + "'");
	s = s.substr(1);
	return key;
}

void JsonObject::parseJsonObject(std::string &s, const std::string &object_key)
{
	JsonObject json;
}

JsonObject::JsonObject()
{

}

void	parseFromText(const std::string &text)
{

}

JsonObject::JsonObject(const std::string &text)
{
	std::string s(text);

	if (s.size() == 0)
		throw std::logic_error("JSON error : empty string");
	s = ltrim(rtrim(s));
	if (s.size() == 0 || s[0] != '{')
		throw std::logic_error("JSON error: not an JSON object");
	s = s.substr(1, s.size() - 3);

	std::string key;
	do
	{
		key = parseJsonKey(s);
		if(s[0] == '{')
			this->parseJsonObject(key, s);
		else if ('[')
			this->parseJsonArray(key, s);
		else if (s[0] == '"')
			this->parseJsonString(key, s);
		else if (std::isdigit(s[0]) || s[0] == '-')
			this->parseJsonNumber(key, s);
		else
			throw std::logic_error("JSON error: Unknown value type for key '" + key + "'");


		std::cout << "key = '" << key << "'" << std::endl;


	}
	//while (false);
	while(!key.empty());

}

JsonObject::JsonObject(const JsonObject &rhs)
{
	*this = rhs;
}

JsonObject &JsonObject::operator=(const JsonObject &rhs)
{
	this->intArrays.clear();
	this->stringArrays.clear();
	this->objectsArrays.clear();
	this->strings.clear();
	this->ints.clear();
	//TODO TYR : implement
}

JsonObject::~JsonObject()
{
	//TODO
}

void JsonObject::parseJsonNumber(const std::string &name, std::string &s)
{
	size_t i(0);
	std::string str = s;
	bool isFloat(false);
	str = ltrim(rtrim(str));
	if (str[i] == '-')
		i++;
	if (!std::isdigit(str[i]))
		throw std::logic_error("JSON error parsing key '" + name + "' (no digit)");
	while(std::isdigit(str[i]))
		i++;
	if (str[i] == '.')
	{
		if (!std::isdigit(str[i+1]))
			throw std::logic_error("JSON error parsing key '" + name + "' (no decimal)");
		isFloat = true;
		i++;
		while(std::isdigit(str[i]))
			i++;
	}
	// TODO TYR: handle scientific power

	s.assign(s, i + 1);

	if (isFloat)
	{
		float valueFloat(0.0f);
		std::istringstream ( str ) >> valueFloat;
		this->floats.insert(std::pair<std::string, float>(name, valueFloat));
	}
	else
	{
		int valueInt(0);
		std::istringstream ( str ) >> valueInt;
		this->ints.insert(std::pair<std::string, int>(name, valueInt));
	}
}
