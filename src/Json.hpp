#include "../includes/webserv.hpp"
#include <exception>
#include <vector>

class JsonObject;
class JsonArray;

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
		std::vector<int> intValues() { return this->ints;}
		std::vector<std::string> stringValues() { return this->strings;}
		std::vector<bool> boolValues() { return this->bools;}
		std::vector<JsonObject> ObjectValues() {return this->objects;}
		std::vector<JsonArray> arrayValues() {return this->arrays;}

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

		JsonArray					getArray(const std::string &name) const;
		JsonArray					getArrayOrEmpty(const std::string &name) const;
		JsonObject					getObject(const std::string &name) const;
		std::string				 	getString(const std::string &name) const;
		std::string					getStringOrDefault(const std::string &name, const std::string &val) const;
		bool					 	getBool(const std::string &name) const;
		bool					 	getBoolOrDefault(const std::string &name, bool val) const;
		int						 	getInt(const std::string &name) const;
		int							getIntOrDefault(const std::string &name, int val) const;
};

std::ostream & operator<<(std::ostream &os, const JsonObject &json);
