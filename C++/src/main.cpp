#include "input.hpp"
#include "main_menu.hpp"

#include "modules/calculate_fulction_values/calculate_fulction_values.hpp"
#include "modules/game/game.hpp"
#include "modules/math_sum/math_sum.hpp"
//#include "modules/prime_numbers/prime_numbers.hpp"

int main() {
	while (true) {
		printMainMenu();
		int value = getValue<int>("Введите раздел программы");
		MainMenu element;
		try {
			element = convertIntToMainMenu(value);
		} catch (...) {
			continue;
		}
		switch (element) {
		case MainMenu::CALCULATE_FUNCTION_VALUE:
			calculateFunctionValues();
			break;
		case MainMenu::PRIME_NUMBERS:
			//calculatePrimeNumbers();
			break;
		case MainMenu::MATH_SUM:
			calculateMathSum();
			break;
		case MainMenu::GAME:
			startGame();
			break;
		case MainMenu::EXIT:
			return 0;
		}
	}
	return 0;
}
