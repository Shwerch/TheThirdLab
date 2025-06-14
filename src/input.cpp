#include "input.hpp"

#include <limits>
#include <string>

void clearInput() {
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string getLine(const std::string &message) {
	clearInput();
	std::string input;
	std::cout << message << ": ";
	std::getline(std::cin, input);
	std::cout << std::endl;
	return input;
}
