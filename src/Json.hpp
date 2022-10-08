#include "../includes/webserv.hpp"
#include <exception>
#include <vector>

bool isValidJsonArray(const std::string &s, bool checkStartOnly = false);
bool isValidJsonObject(std::string s, bool checkStartOnly = false);

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
		std::map<std::string, std::vector<int>> intArrays;
		std::map<std::string, std::vector<std::string>> stringArrays;
		std::map<std::string, std::vector<JsonObject>> objectsArrays;
		std::map<std::string, std::string> strings;
		std::map<std::string, JsonObject> objects;
		std::map<std::string, int> ints;
		std::map<std::string, float> floats;
		std::map<std::string, bool> bools;

		std::string parseJsonKey(std::string &s);

		void parseJsonNumber(std::string &s, const std::string &key);
		void parseJsonString(std::string &s, const std::string &key);
		void parseJsonObject(std::string &s, const std::string &key);
		void parseJsonArray(std::string &s, const std::string &key);
		void parseJsonConstant(std::string &s, const std::string &key);

		friend std::ostream & operator<<(std::ostream &os, const JsonObject &json);

		void	parseObjectFromString(std::string &text);

	public:

		JsonObject();
		JsonObject(const std::string &text);
		JsonObject(const JsonObject &rhs);
		JsonObject &operator=(const JsonObject &rhs);
		~JsonObject();

		void	parseObjectFromFile(const std::string &file);

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
