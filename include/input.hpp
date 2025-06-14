#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void clearInput();

template <typename T> T getValue(const std::string &message) {
	T value;
	while (true) {
		std::cout << message << ": ";
		std::cin >> value;
		if (std::cin.good()) {
			break;
		}
		clearInput();
	}
	return value;
}

std::string getLine(const std::string &message);

template <typename T> std::vector<T> getVector(const std::string &message) {
	std::vector<T> result;
	std::string line;

	while (true) {
		clearInput();
		std::cout << message << ": ";
		std::getline(std::cin, line);
		std::stringstream ss(line);
		int num;
		result.clear();
		while (ss >> num) {
			result.push_back(num);
		}
		if (!result.empty() && ss.eof()) {
			break;
		}
	}
	return result;
}
