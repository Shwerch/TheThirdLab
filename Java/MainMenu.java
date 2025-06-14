// Content of src/MainMenu.java
import java.util.InputMismatchException;
import java.util.Scanner;

enum MainMenu {
	CALCULATE_FUNCTION_VALUE("Вычисление значений функции"),
	PRIME_NUMBERS("Тесты на простоту чисел"),
	MATH_SUM("Вычисление ряда"),
	GAME("Увлекательная игра"),
	EXIT("Выход из программы");

	private final String description;

	MainMenu(String description) {
		this.description = description;
	}

	public String getDescription() {
		return description;
	}

	public static int getMainMenuElements() {
		return MainMenu.values().length;
	}

	public static MainMenu convertIntToMainMenu(int element) {
		if (element < 1 || element > MainMenu.values().length) {
			throw new IllegalArgumentException("Некорректный ввод");
		}
		return MainMenu.values()[element - 1];
	}

	public static void printMainMenu() {
		for (int i = 0; i < MainMenu.values().length; i++) {
			System.out.println(
				"[" + (i + 1) + "] " + MainMenu.values()[i].getDescription()
			);
		}
		System.out.println();
	}
}
