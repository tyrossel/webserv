#include "../includes/webserv.hpp"
#include <exception>
#include <vector>

class JsonObject;
class JsonArray;

// TODO: DO NOT EXPOSE THOSE
int			parseJsonInt(std::string &s, const std::string &key);
std::string	parseJsonString(std::string &s, const std::string &key);
JsonObject	parseJsonObject(std::string &s, const std::string &key);
JsonArray	parseJsonArray(std::string &s, const std::string &key);
bool		parseJsonConstant(std::string &s, const std::string &key);

class JsonArray
{
	public:

		enum ArrayType {
			NONE,
			INT,
			STRING,
			BOOL,
			OBJECT,
			ARRAY
		};

		JsonArray();
		JsonArray(const std::string &text);
		JsonArray(const JsonArray &rhs);
		JsonArray &operator=(const JsonArray &rhs);
		~JsonArray();

		void	clear();

		template<typename T>
		std::vector<T> value()
		{
			throw std::logic_error("Json array error: cannot contain this type");
		}
		template<typename T>
		std::vector<int> value() { return this->ints;}
		template<typename T>
		std::vector<std::string> value() { return this->strings;}
		template<typename T>
		std::vector<bool> value() { return this->bools;}
		template<typename T>
		std::vector<JsonObject> value() {return this->objects;}
		template<typename T>
		std::vector<JsonArray> value() {return this->arrays;}

		void parseFromString(std::string &s);

	private:

		void		parseJsonValue(std::string &s, const std::string &key);

		ArrayType type;
		std::vector<int>	ints;
		std::vector<std::string>	strings;
		std::vector<bool> bools;
		std::vector<JsonObject> objects;
		std::vector<JsonArray> arrays;

		friend std::ostream & operator<<(std::ostream &os, const JsonArray &json);
};

//-------------------------------------------------------------------------------
class JsonObject
{
	public:
	class JsonError : public std::exception{
		const std::string _msg;
		public:
			JsonError(const std::string &msg) : _msg(msg){}
			~JsonError()throw(){}
			virtual const char * what() const throw ()
			{
				return (_msg.c_str());
			}
	};

	private:

		std::map<std::string, JsonArray> arrays;
		std::map<std::string, std::string> strings;
		std::map<std::string, JsonObject> objects;
		std::map<std::string, int> ints;
		std::map<std::string, float> floats;
		std::map<std::string, bool> bools;

		std::string parseJsonKey(std::string &s);

		void		parseJsonValue(std::string &s, const std::string &key);

		friend std::ostream & operator<<(std::ostream &os, const JsonObject &json);


	public:

		JsonObject();
		JsonObject(const std::string &text);
		JsonObject(const JsonObject &rhs);
		JsonObject &operator=(const JsonObject &rhs);
		~JsonObject();

		void	clear();
		void	parseFromString(std::string &text);
		void	parseFromFile(const std::string &file);

		JsonObject getObject(std::string name);

		std::vector<int>			getIntArray(std::string name);
		std::vector<std::string>	getStringArray(std::string name);
		std::vector<JsonObject>		getObjectArray(std::string name);
		std::string				 	getString(std::string name);
		bool					 	getBool(std::string name);
		int						 	getInt(std::string name);
		float					 	getFloat(std::string name);
};

std::ostream & operator<<(std::ostream &os, const JsonObject &json);
