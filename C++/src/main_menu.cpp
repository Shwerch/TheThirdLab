#include "main_menu.hpp"

#include <iostream>
#include <stdexcept>

MainMenu convertIntToMainMenu(int element) {
	element--;
	if (element > static_cast<int>(MainMenu::EXIT) || element < 0) {
		throw std::runtime_error("Некорректный ввод");
	}
	return static_cast<MainMenu>(element);
}

void printMainMenu() {
	for (int i = 0; i < mainMenuElements; i++) {
		std::cout << "[" << i + 1 << "] " << getElementSescription(static_cast<MainMenu>(i))
				  << std::endl;
	}
	std::cout << std::endl;
}
