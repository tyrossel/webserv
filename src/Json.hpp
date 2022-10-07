#include "../includes/webserv.hpp"
#include <exception>
#include <vector>

bool isValidJsonNumber(const std::string &s, bool checkStartOnly = false);
bool isValidJsonString(const std::string &s, bool checkStartOnly = false);
bool isValidJsonConstant(const std::string &s, bool checkStartOnly = false);
bool isValidJsonArray(const std::string &s, bool checkStartOnly = false);
bool isValidJsonObject(std::string s, bool checkStartOnly = false);
bool isValidJsonValue(const std::string &s, bool checkStartOnly = false);

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
		std::map<std::string, int> ints;
		std::map<std::string, float> floats;
		std::map<std::string, bool> bools;

	public:

		std::string parseJsonKey(std::string &s);

		void parseJsonNumber(std::string &s, const std::string &key);
		void parseJsonString(std::string &s, const std::string &key);
		void parseJsonObject(std::string &s, const std::string &key);
		void parseJsonArray(std::string &s, const std::string &key);
		void parseJsonBool(std::string &s, const std::string &key);

		JsonObject();
		void	parseFromText(const std::string &text);
		JsonObject(const JsonObject &rhs);
		JsonObject &operator=(const JsonObject &rhs);
		~JsonObject();

		std::vector<std::string> getObject(std::string name);

		template<class T>
		T 							getValue(std::string name);
		std::vector<int>			getIntArray(std::string name);
		std::vector<std::string>	getStringArray(std::string name);
		std::vector<JsonObject>		getObjectArray(std::string name);
		std::string				 	getString(std::string name);
		bool					 	getBool(std::string name);
		int						 	getInt(std::string name);
		float					 	getFloat(std::string name);
};
