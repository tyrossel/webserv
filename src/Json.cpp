#include "Json.hpp"
#include "Utils.hpp"
#include <cctype>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <iomanip>

static JsonObject parseJsonObject(std::string &s, const std::string &key)
{
	JsonObject json;

	(void)key;
	json.parseFromString(s);
	return json;
}

static JsonArray parseJsonArray(std::string &s, const std::string &key)
{
	JsonArray array;

	(void)key;
	array.parseFromString(s);
	return array;
}

static int parseJsonInt(std::string &s, const std::string &key)
{
	size_t i(0);
	std::string str = s;
	ft::trimLeft(s);
	ft::trimRight(s);
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
		i++;
		while(std::isdigit(str[i]))
			i++;
	}
	// TODO TYR: handle scientific power

	s.assign(s, i);

	int valueInt(0);
	std::istringstream ( str ) >> valueInt;
	return valueInt;
}

static bool parseJsonConstant(std::string &s, const std::string &key)
{
	if (s.substr(0, 4) == "true")
	{
		s.assign(s, 4);
		return true;
	}
	else if (s.substr(0, 5) == "false")
	{
		s.assign(s, 5);
		return false;
	}
	else if (s.substr(0, 4) == "null")
	{
		// TODO TYR: Handle NULL
		s.assign(s, 4);
		return false;
	}
	else
		throw std::logic_error("JSON error: Unknown value type for key '" + key + "' : '" + s[0] + "'");
}

static std::string parseJsonString(std::string &s, const std::string &key)
{
	size_t i(0);
	std::string str(s), content;
	size_t last_quote = 0;

	ft::trimLeft(s);
	ft::trimRight(s);
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
	s.assign(s, i + 1);
	return content;
}

//-------------------------------------------------------------------------------
// JsonArray
// ------------------------------------------------------------------------------
JsonArray::JsonArray() : type(NONE)
{

}

JsonArray::JsonArray(const JsonArray &rhs)
{
	*this = rhs;
}

JsonArray &JsonArray::operator=(const JsonArray &rhs)
{
	if (&rhs ==  this)
		return *this;
	this->clear();
	this->type = rhs.type;
	{
		std::vector<JsonArray>::const_iterator it;
		for (it = rhs.arrays.begin(); it != rhs.arrays.end(); it++)
			this->arrays.push_back(*it);
	}
	{
		std::vector<int>::const_iterator it;
		for (it = rhs.ints.begin(); it != rhs.ints.end(); it++)
			this->ints.push_back(*it);
	}
	{
		std::vector<bool>::const_iterator it;
		for (it = rhs.bools.begin(); it != rhs.bools.end(); it++)
			this->bools.push_back(*it);
	}
	{
		std::vector<std::string>::const_iterator it;
		for (it = rhs.strings.begin(); it != rhs.strings.end(); it++)
			this->strings.push_back(*it);
	}
	{
		std::vector<JsonObject>::const_iterator it;
		for (it = rhs.objects.begin(); it != rhs.objects.end(); it++)
			this->objects.push_back(*it);
	}
	return *this;
}

JsonArray::~JsonArray()
{
	this->clear();
}

void JsonArray::clear()
{
	this->ints.clear();
	this->strings.clear();
	this->objects.clear();
	this->arrays.clear();
	this->type = NONE;
}

void JsonArray::parseFromString(std::string &s)
{
	ft::trimLeft(s);
	if (s.size() == 0 || s[0] != '[')
		throw std::logic_error("JSON array error: not an array");
	s = s.assign(s, 1);
	ft::trimLeft(s);

	do
	{
		this->parseJsonValue(s, "");
		ft::trimLeft(s);
		if (s[0] == ',')
		{
			s = s.assign(s, 1);
			ft::trimLeft(s);
		}
		else if (s[0] != ']')
			throw std::logic_error("JSON array error: missing comma or closing bracket : '" + s + "'");
	}
	while(!s.empty() && s[0] != ']');

	if (s[0] != ']')
		throw std::logic_error("JSON array error: no closing bracket");
	s = s.assign(s, 1);
	ft::trimLeft(s);
}

void JsonArray::parseJsonValue(std::string &s, const std::string &key)
{
		if(s[0] == '{')
		{
			JsonObject object = parseJsonObject(s, key);
			this->objects.push_back(object);
			if (type == NONE)
				type = OBJECT;
		}
		else if (s[0] == '[')
		{
			JsonArray array = parseJsonArray(s, key);
			this->arrays.push_back(array);
			if (type == NONE)
				type = ARRAY;
		}
		else if (s[0] == '"')
		{
			std::string value = parseJsonString(s, key);
			this->strings.push_back(value);
			if (type == NONE)
				type = STRING;
		}
		else if (std::isdigit(s[0]) || s[0] == '-')
		{
			int nbr = parseJsonInt(s, key);
			this->ints.push_back(nbr);
			if (type == NONE)
				type = INT;
		}
		else
		{
			bool b = parseJsonConstant(s, key);
			this->bools.push_back(b);
			if (type == NONE)
				type = BOOL;
		}
}


//-------------------------------------------------------------------------------
// JsonObject
// ------------------------------------------------------------------------------
JsonObject::JsonObject()
{
}

JsonObject::JsonObject(const std::string &text)
{
	std::string s(text);

	parseFromString(s);
}

JsonObject::JsonObject(const JsonObject &rhs)
{
	*this = rhs;
}

JsonObject &JsonObject::operator=(const JsonObject &rhs)
{
	if (&rhs == this) // NOTE TYR: should use the copy & swap idiom
		return *this;
	this->clear();
	{
		std::map<std::string, JsonArray>::const_iterator it;
		for (it = rhs.arrays.begin(); it != rhs.arrays.end(); it++)
			this->arrays.insert(*it);
	}
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
}

std::string	JsonObject::parseJsonKey(std::string &s)
{
	size_t end_of_key;
	std::string key("");

	ft::trimLeft(s);
	if(s[0] != '"')
		throw std::logic_error("Key doesnt start with quote:");
	end_of_key = s.find_first_of('"', 1);
	if (end_of_key == std::string::npos)
		throw std::logic_error("No end of key");
	key = s.substr(1, end_of_key - 1);
	s = s.substr(end_of_key + 1);
	ft::trimLeft(s);
	if (s[0] != ':')
		throw std::logic_error("JSON error: no colon after key '" + key + "'");
	s = s.assign(s, 1);
	ft::trimLeft(s);
	return key;
}


void JsonObject::parseJsonValue(std::string &s, const std::string &key)
{
		if(s[0] == '{')
		{
			JsonObject object = parseJsonObject(s, key);
			this->objects[key] = object;
		}
		else if (s[0] == '[')
		{
			JsonArray array = parseJsonArray(s, key);
			this->arrays[key] = array;
		}
		else if (s[0] == '"')
		{
			std::string value = parseJsonString(s, key);
			this->strings[key] = value;
		}
		else if (std::isdigit(s[0]) || s[0] == '-')
		{
			int nbr = parseJsonInt(s, key);
			this->ints[key] = nbr;
		}
		else
		{
			bool b = parseJsonConstant(s, key);
			this->bools[key] = b;
		}
}

void	JsonObject::parseFromFile(const std::string &file)
{
	std::string text = ft::readFile(file);

	this->parseFromString(text);
}

void	JsonObject::clear()
{
	this->arrays.clear();
	this->strings.clear();
	this->objects.clear();
	this->ints.clear();
	this->floats.clear();
	this->bools.clear();
}

void	JsonObject::parseFromString(std::string &s)
{
	this->clear();

	if (s.size() == 0)
		throw std::logic_error("JSON error : empty string");
	ft::trimLeft(s);
	ft::trimRight(s);
	if (s.size() == 0 || s[0] != '{')
		throw std::logic_error("JSON error: not an JSON object");
	s = s.substr(1);

	std::string key;
	do
	{
		key = parseJsonKey(s);
		parseJsonValue(s, key);
		ft::trimLeft(s);
		if (s[0] == ',')
		{
			s = s.assign(s, 1);
			ft::trimLeft(s);
		}
		else if (s[0] != '}')
			throw std::logic_error("JSON objet error: missing comma or closing bracket after key " + key);
	}
	while(!s.empty() && s[0] != '}');

	if (s[0] != '}')
		throw std::logic_error("JSON object error: no closing bracket");
	s = s.assign(s, 1);
	ft::trimLeft(s);
}

JsonArray	JsonObject::getArray(const std::string &name) const
{
	std::map<std::string, JsonArray>::const_iterator it;
	for (it = this->arrays.begin(); it != this->arrays.end(); it++)
	{
		if (name == it->first)
			return it->second;
	}
	throw std::logic_error("JSON error: no array named " + name);
}

JsonArray	JsonObject::getArrayOrEmpty(const std::string &name) const
{
	std::map<std::string, JsonArray>::const_iterator it;
	for (it = this->arrays.begin(); it != this->arrays.end(); it++)
	{
		if (name == it->first)
			return it->second;
	}
	return JsonArray();
}

JsonObject	JsonObject::getObject(const std::string &name) const
{
	std::map<std::string, JsonObject>::const_iterator it;
	for (it = this->objects.begin(); it != this->objects.end(); it++)
	{
		if (name == it->first)
			return it->second;
	}
	throw std::logic_error("JSON error: no object named " + name);
}

JsonObject	JsonObject::getObjectOrEmpty(const std::string &name) const
{
	std::map<std::string, JsonObject>::const_iterator it;
	for (it = this->objects.begin(); it != this->objects.end(); it++)
	{
		if (name == it->first)
			return it->second;
	}
	return JsonObject();
}

std::string	JsonObject::getString(const std::string &name) const
{
	if (this->strings.find(name) == this->strings.end())
		throw std::logic_error("JSON error: no string named " + name);
	return this->strings.at(name);
}

std::string	JsonObject::getStringOrDefault(const std::string &name, const std::string &val) const
{
	if (this->strings.find(name) == this->strings.end())
		return val;
	return this->strings.at(name);
}

int	JsonObject::getInt(const std::string &name) const
{
	if (this->ints.find(name) == this->ints.end())
		throw std::logic_error("JSON error: no int named " + name);
	return this->ints.at(name);
}

int	JsonObject::getIntOrDefault(const std::string &name, int val) const
{
	if (this->ints.find(name) == this->ints.end())
		return val;
	return this->ints.at(name);
}

std::map<std::string, JsonArray>	JsonObject::getAllArrays() const
{
	return this->arrays;
}

bool	JsonObject::getBool(const std::string &name) const
{
	if (this->bools.find(name) == this->bools.end())
		throw std::logic_error("JSON error: no bool named " + name);
	return this->bools.at(name);

}

bool	JsonObject::getBoolOrDefault(const std::string &name, bool val) const
{
	if (this->bools.find(name) == this->bools.end())
		return val;
	return this->bools.at(name);
}

//------------------------------------------------------------------------------
// Operators <<
//------------------------------------------------------------------------------

std::ostream & operator<<(std::ostream &os, const JsonArray &json)
{
	bool print_comma = false;

	os << "[";
	if (json.type == JsonArray::INT)
	{
		std::vector<int>::const_iterator it;
		for (it = json.ints.begin(); it != json.ints.end(); it++)
		{
			if (!print_comma)
				print_comma = true;
			else
				os << ", ";
			os << *it;
		}
	}
	else if (json.type == JsonArray::BOOL)
	{
		std::vector<bool>::const_iterator it;
		for (it = json.bools.begin(); it != json.bools.end(); it++)
		{
			if (!print_comma)
				print_comma = true;
			else
				os << ", ";
			os << *it;
		}
	}
	else if (json.type == JsonArray::STRING)
	{
		std::vector<std::string>::const_iterator it;
		for (it = json.strings.begin(); it != json.strings.end(); it++)
		{
			if (!print_comma)
				print_comma = true;
			else
				os << ", ";
			os << "\"" << *it << "\"";
		}
	}
	else if (json.type == JsonArray::ARRAY)
	{
		std::vector<JsonArray>::const_iterator it;
		for (it = json.arrays.begin(); it != json.arrays.end(); it++)
		{
			if (!print_comma)
				print_comma = true;
			else
				os << ", ";
			os << *it;
		}
	}
	else if (json.type == JsonArray::OBJECT)
	{
		std::vector<JsonObject>::const_iterator it;
		for (it = json.objects.begin(); it != json.objects.end(); it++)
		{
			if (!print_comma)
				print_comma = true;
			else
				os << ", ";
			os << *it;
		}
	}
	os << "]";
	return os;
}

std::ostream & operator<<(std::ostream &os, const JsonObject &json)
{
	unsigned int first_col_size = 10;
	os << "{" << std::endl;
	{
		std::map<std::string, int>::const_iterator it;
		for (it = json.ints.begin(); it != json.ints.end(); it++)
			os << std::setw(first_col_size) << "int: " << it->first << " : " << it->second << std::endl;
	}
	{
		std::map<std::string, float>::const_iterator it;
		for (it = json.floats.begin(); it != json.floats.end(); it++)
			os << std::setw(first_col_size) << "float: " << it->first << " : " << it->second << std::endl;
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
	{
		std::map<std::string, JsonArray>::const_iterator it;
		for (it = json.arrays.begin(); it != json.arrays.end(); it++)
			os << std::setw(first_col_size) << "arrays: " << it->first << " : " << it->second << std::endl;
	}
	os << "}";
	return os;
}
