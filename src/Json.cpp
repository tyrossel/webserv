#include "Json.hpp"
#include <cctype>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <iomanip>

static inline std::string &ltrim(std::string &s)
{
	return s.erase(0, s.find_first_not_of(" \t\n\v\f\r"));
}

static inline std::string &rtrim(std::string &s)
{
	return s.erase(s.find_last_not_of(" \t\n\v\f\r") + 1);
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

JsonObject::JsonObject()
{

}

JsonObject::JsonObject(const std::string &text)
{
	std::string s(text);

	parseObjectFromString(s);
}

JsonObject::JsonObject(const JsonObject &rhs)
{
	*this = rhs;
}

JsonObject &JsonObject::operator=(const JsonObject &rhs)
{
	if (&rhs == this) // NOTE TYR: should use the copy & swap idiom
		return *this;
	this->intArrays.clear();
	this->stringArrays.clear();
	this->objectsArrays.clear();
	this->strings.clear();
	this->ints.clear();
	{
		std::map<std::string, int>::const_iterator it;
		for (it = rhs.ints.begin(); it != rhs.ints.end(); it++)
			this->ints.insert(*it);
	}
	{
		std::map<std::string, float>::const_iterator it;
		for (it = rhs.floats.begin(); it != rhs.floats.end(); it++)
			this->floats.insert(*it);
	}
	{
		std::map<std::string, bool>::const_iterator it;
		for (it = rhs.bools.begin(); it != rhs.bools.end(); it++)
			this->bools.insert(*it);
	}
	{
		std::map<std::string, std::string>::const_iterator it;
		for (it = rhs.strings.begin(); it != rhs.strings.end(); it++)
			this->strings.insert(*it);
	}
	{
		std::map<std::string, JsonObject>::const_iterator it;
		for (it = rhs.objects.begin(); it != rhs.objects.end(); it++)
			this->objects.insert(*it);
	}
	//TODO TYR : implement arrays
	return *this;
}

JsonObject::~JsonObject()
{
	//TODO
}
std::string	JsonObject::parseJsonKey(std::string &s)
{
	size_t end_of_key;
	std::string key("");

	s = ltrim(s);
	// std::cout << "parse key: s = " << s << std::endl;
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
	s = ltrim(s.assign(s, 1));
	return key;
}

void JsonObject::parseJsonObject(std::string &s, const std::string &object_key)
{
	JsonObject json;

	json.parseObjectFromString(s);
}

void	JsonObject::parseObjectFromFile(const std::string &file)
{
	std::ifstream fs(file.c_str());

	if (!fs.good())
		throw std::logic_error("JSON error: file '" + file + "' cannot be opened");
	std::string text;
	text.assign(std::istreambuf_iterator<char>(fs),
			std::istreambuf_iterator<char>());
	fs.close();

	this->parseObjectFromString(text);
}

void	JsonObject::parseObjectFromString(std::string &s)
{
	if (s.size() == 0)
		throw std::logic_error("JSON error : empty string");
	s = ltrim(rtrim(s));
	if (s.size() == 0 || s[0] != '{')
		throw std::logic_error("JSON error: not an JSON object");
	s = s.substr(1);

	std::string key;
	do
	{
		key = parseJsonKey(s);
		std::cout << "Key = " << key << std::endl;
		// std::cout << "Start keys loop: s = " << s << std::endl;
		if(s[0] == '{')
		{
			JsonObject json;
			json.parseObjectFromString(s);
			this->objects.insert(std::pair<std::string, JsonObject>(key, json));
			this->parseJsonObject(key, s);
		}
		// else if ('[')
		// 	this->parseJsonArray(key, s);
		else if (s[0] == '"')
			this->parseJsonString(s, key);
		else if (std::isdigit(s[0]) || s[0] == '-')
			this->parseJsonNumber(s, key);
		else
			this->parseJsonConstant(s, key);
		s = ltrim(s);
		if (s[0] == ',')
			s = ltrim(rtrim(s.assign(s, 1)));
		else if (s[0] != '}')
			throw std::logic_error("JSON objet error: missing comma or closing bracket after key " + key + ": s = " + s);
		// std::cout << "End keys loop: s = " << s << std::endl;
	}
	while(!s.empty() && s[0] != '}');

	if (s[0] != '}')
		throw std::logic_error("JSON object error: no closing bracket");

}

void JsonObject::parseJsonNumber(std::string &s, const std::string &key)
{
	size_t i(0);
	std::string str = s;
	bool isFloat(false);
	str = ltrim(rtrim(str));
	// std::cout << "parse number: s = " << s << std::endl;
	if (str[i] == '-')
		i++;
	if (!std::isdigit(str[i]))
		throw std::logic_error("JSON error parsing key '" + key + "' (no digit)");
	while(std::isdigit(str[i]))
		i++;
	if (str[i] == '.')
	{
		if (!std::isdigit(str[i+1]))
			throw std::logic_error("JSON error parsing key '" + key + "' (no decimal)");
		isFloat = true;
		i++;
		while(std::isdigit(str[i]))
			i++;
	}
	// TODO TYR: handle scientific power

	s.assign(s, i);

	if (isFloat)
	{
		float valueFloat(0.0f);
		std::istringstream ( str ) >> valueFloat;
		this->floats.insert({key, valueFloat});
		std::cout << "Number is: " << valueFloat << std::endl;
	}
	else
	{
		int valueInt(0);
		std::istringstream ( str ) >> valueInt;
		this->ints.insert({key, valueInt});
		std::cout << "Number is: " << valueInt << std::endl;
	}
}

void JsonObject::parseJsonConstant(std::string &s, const std::string &key)
{
	if (s.substr(0, 4) == "true")
	{
		this->bools.insert({key, true});
		s.assign(s, 4);
	}
	else if (s.substr(0, 5) == "false")
	{
		this->bools.insert({key, false});
		s.assign(s, 5);
	}
	else if (s.substr(0, 4) == "null")
	{
		// TODO TYR: Handle NULL
		s.assign(s, 4);
	}
	else
		throw std::logic_error("JSON error: Unknown value type for key '" + key + "' : '" + s[0] + "'");
}

void JsonObject::parseJsonString(std::string &s, const std::string &key)
{
	int i = 0;
	std::string str(s), content;
	size_t last_quote = 0;

	str = ltrim(rtrim(str));
	if (s.size() == 0 || s[0] != '"')
		throw std::logic_error("JSON string: error parsing key '" + key + "' (no string");
	do {
		last_quote = str.find_first_of('"', last_quote + 1);
	} while (last_quote != std::string::npos && str[last_quote - 1] == '\\');
	if (last_quote == std::string::npos)
		throw std::logic_error("JSON string: error parsing key '" + key + "' (quote not closed");
	if(str.at(i) == '"')
		i++;
	while(i < s.size() && str[i] != '"')
	{
		if (str[i] < 32 || str[i] > 126)
			throw std::logic_error("JSON string: error parsing key '" + key + " (non-printable character)");
		// TODO TYR: handle "\uABCD"
		if (str[i] == '\\')
		{
			switch (str[i+1])
			{
				case '\"':
				case '\\':
				case '/':
					content += str[i+1];
					break;
				case 'b':
					content += '\b';
					break;
				case 'f':
					content += '\f';
					break;
				case 'n':
					content += '\n';
					break;
				case 'r':
					content += '\r';
					break;
				case 't':
					content += '\t';
					break;
				default:
					throw std::logic_error("JSON string: error parsing key '" + key + " (unknown escaped char)");
					break;

			}
			i += 2;
		}
		else
			content += str[i++];
	}
	this->strings.insert(std::pair<std::string, std::string>(key, content));
	s.assign(s, i + 1);
	std::cout << "String is: " << content << std::endl;
}


std::ostream & operator<<(std::ostream &os, const JsonObject &json)
{
	unsigned int first_col_size = 10;

	{
		std::map<std::string, int>::const_iterator it;
		for (it = json.ints.begin(); it != json.ints.end(); it++)
			os << std::setw(first_col_size) << "int: " << it->first << " : " << it->second << std::endl;
	}
	{
		std::map<std::string, bool>::const_iterator it;
		for (it = json.bools.begin(); it != json.bools.end(); it++)
			os << std::setw(first_col_size) << "bool: " << it->first << " : " << it->second << std::endl;
	}
	{
		std::map<std::string, std::string>::const_iterator it;
		for (it = json.strings.begin(); it != json.strings.end(); it++)
			os << std::setw(first_col_size) << "string: " << it->first << " : " << it->second << std::endl;
	}
	return os;
}
