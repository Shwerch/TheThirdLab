public class Main {

	public static void main(String[] args) {
		while (true) {
			MainMenu.printMainMenu();
			Integer value = Input.getValue(
				"Введите раздел программы",
				Integer.class
			);

			MainMenu element;
			try {
				element = MainMenu.convertIntToMainMenu(value);
			} catch (IllegalArgumentException e) {
				System.out.println(e.getMessage());
				continue;
			}

			switch (element) {
				case CALCULATE_FUNCTION_VALUE:
					CalculateFunctionValues.calculateFunctionValues();
					break;
				case PRIME_NUMBERS:
					PrimeNumbers.calculatePrimeNumbers();
					break;
				case MATH_SUM:
					MathSum.calculateMathSum();
					break;
				case GAME:
					Game.startGame();
					break;
				case EXIT:
					System.out.println("Выход из программы.");
					return;
			}
		}
	}
}
