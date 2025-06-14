#pragma once

#include <stdexcept>
#include <string_view>

enum class MainMenu {
	CALCULATE_FUNCTION_VALUE,
	PRIME_NUMBERS,
	MATH_SUM,
	GAME,
	EXIT,
};

constexpr int mainMenuElements = static_cast<int>(MainMenu::EXIT) + 1;

constexpr std::string_view getElementSescription(MainMenu element) {
	switch (element) {
	case MainMenu::CALCULATE_FUNCTION_VALUE:
		return "Вычисление значений функции";
	case MainMenu::PRIME_NUMBERS:
		return "Тесты на простоту чисел";
	case MainMenu::MATH_SUM:
		return "Вычисление ряда";
	case MainMenu::GAME:
		return "Увлекательная игра";
	case MainMenu::EXIT:
		return "Выход из программы";
	default:
		throw std::runtime_error("Как ты это сделал");
	}
}

MainMenu convertIntToMainMenu(int element);

void printMainMenu();
