#include "../src/Json.hpp"

std::string test_numbers[] = {
	"0.0",
	"123",
	"-123",
	"-123.023",
	"123.123",
	"000123",
	"123.",
	"123.a",
	".123",
	"a123",
	"123a",
	"123 a",
	"aaaa"
};

std::string test_strings[] = {
	"\"hello\"",
	"\"\\b\\f\\n\\r\\t\"",
	"\"He\\\\llo\"",
	"\"\\\"\"",
	"\"Hello\\\"",
	"hello",
	"\"Hello\"hello",
	"Hello\"hello\"",
};

std::string test_arrays[] = {
	"[12]",
	"[4, 8, 15, 15, 23, 42]",
	"[12]",
	"[12]",
};

const std::string json_txt = "{ \"test_int\":42}";

int main()
{
	JsonObject json(json_txt);
	// std::cout << "JSON NUMBERS: " << std::endl;
	// for (const std::string &str : test_numbers)
	// 	std::cout << "'" << str << "': " << isValidJsonNumber(str, false) << std::endl;
	// std::cout << std::endl;
	//
	// std::cout << "JSON STRING: " << std::endl;
	// for (const std::string &str : test_strings)
	// 	std::cout << str << ": " << isValidJsonString(str, false) << std::endl;
}
