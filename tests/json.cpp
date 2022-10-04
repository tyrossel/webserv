#include "../src/Json.hpp"

std::string test_numbers[] = {
	"0.0",
	"123",
	"a"
};

int main()
{
	std::cout << "JSON NUMBERS: " << std::endl;
	for (const std::string &str : test_numbers)
	{
		std::cout << "'" << str << "': " << isValidJsonNumber(str) << std::endl;
	}

}
