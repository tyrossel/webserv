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

const std::string json_txt = "{ \"test_int\":42,}";

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cerr << "Usage: ./test_json json_file" << std::endl;
		exit(1);
	}
	try
	{
		JsonObject json;

		json.parseFromFile(argv[1]);

		std::cout << "---------------------" << std::endl;
		std::cout << json << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
