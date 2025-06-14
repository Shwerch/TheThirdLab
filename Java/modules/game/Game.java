import java.util.Collections;
import java.util.HashMap;
import java.util.InputMismatchException;
import java.util.Map;
import java.util.Scanner;
import java.util.Vector;

// Структура для хранения результатов игры
class GameResult {

	public long pavelScore;
	public long vikaScore;

	public GameResult(long pavelScore, long vikaScore) {
		this.pavelScore = pavelScore;
		this.vikaScore = vikaScore;
	}

	// For memoization, to indicate an uncomputed state
	public static GameResult uncomputed() {
		return new GameResult(-1, -1);
	}

	public boolean isUncomputed() {
		return pavelScore == -1 && vikaScore == -1;
	}
}

public class Game {

	// Using a Map for memoization, mapping currentNumbers (as a String for key) and
	// turn to GameResult
	private static Map<String, Map<Boolean, GameResult>> memo;

	// Helper to create a unique key for memoization
	private static String getMemoKey(final Vector<Integer> numbers) {
		StringBuilder sb = new StringBuilder();
		for (int num : numbers) {
			sb.append(num).append(",");
		}
		return sb.toString();
	}

	// Рекурсивная функция для определения оптимального хода
	public static GameResult solveGame(
		final Vector<Integer> currentNumbers,
		int maxEraseCount,
		boolean isPavelTurn
	) {
		// Базовый случай: если чисел не осталось, игра окончена
		if (currentNumbers.isEmpty()) {
			return new GameResult(0, 0);
		}

		// Проверка мемоизации
		String key = getMemoKey(currentNumbers);
		if (memo.containsKey(key) && memo.get(key).containsKey(isPavelTurn)) {
			GameResult result = memo.get(key).get(isPavelTurn);
			if (!result.isUncomputed()) {
				return result;
			}
		}

		long bestPavelScore = (isPavelTurn ? Long.MIN_VALUE : Long.MAX_VALUE); // Инициализация для Павла
		long bestVikaScore = (isPavelTurn ? Long.MAX_VALUE : Long.MIN_VALUE); // Инициализация для Вики

		// Игроки действуют оптимально, поэтому выбирают ход, который
		// максимизирует/минимизирует их счет
		for (
			int i = 1;
			i <= Math.min(maxEraseCount, currentNumbers.size());
			++i
		) {
			// Вычисление суммы стираемых чисел
			long currentSum = 0;
			for (int j = 0; j < i; ++j) {
				currentSum += currentNumbers.get(j);
			}

			// Создание новой последовательности чисел после хода
			Vector<Integer> nextNumbers = new Vector<>(
				currentNumbers.subList(i, currentNumbers.size())
			);

			// Рекурсивный вызов для следующего хода
			GameResult nextResult = solveGame(
				nextNumbers,
				maxEraseCount,
				!isPavelTurn
			);

			// Обновление счетов
			if (isPavelTurn) {
				long pavelCandidateScore = currentSum + nextResult.pavelScore;
				long vikaCandidateScore = nextResult.vikaScore;

				// Павел максимизирует свой счет
				if (pavelCandidateScore > bestPavelScore) {
					bestPavelScore = pavelCandidateScore;
					bestVikaScore = vikaCandidateScore;
				} else if (pavelCandidateScore == bestPavelScore) {
					// Если счета равны, Павел выбирает ход, который минимизирует счет Вики
					if (vikaCandidateScore < bestVikaScore) {
						bestVikaScore = vikaCandidateScore;
					}
				}
			} else {
				long pavelCandidateScore = nextResult.pavelScore;
				long vikaCandidateScore = currentSum + nextResult.vikaScore;

				// Вика максимизирует свой счет (что для Павла означает минимизацию разницы)
				if (vikaCandidateScore > bestVikaScore) {
					bestVikaScore = vikaCandidateScore;
					bestPavelScore = pavelCandidateScore;
				} else if (vikaCandidateScore == bestVikaScore) {
					// Если счета равны, Вика выбирает ход, который минимизирует счет Павла
					if (pavelCandidateScore < bestPavelScore) {
						bestPavelScore = pavelCandidateScore;
					}
				}
			}
		}

		// Сохранение результата в мемо
		memo
			.computeIfAbsent(key, k -> new HashMap<>())
			.put(isPavelTurn, new GameResult(bestPavelScore, bestVikaScore));
		return new GameResult(bestPavelScore, bestVikaScore);
	}

	// Вычисляет победителя игры
	public static int calculateWinner(
		final Vector<Integer> numbers,
		int maxEraseCount
	) {
		memo = new HashMap<>(); // Clear memoization table for each new game
		// Решение игры, начиная с хода Павла
		GameResult finalResult = solveGame(numbers, maxEraseCount, true);

		// Определение победителя
		if (finalResult.pavelScore > finalResult.vikaScore) {
			return 1; // Павел победил
		} else {
			return 0; // Вика победила (или ничья)
		}
	}

	public static void startGame() {
		Scanner scanner = new Scanner(System.in);

		// Ввод количества чисел и максимально стираемых чисел
		int totalNumbers;
		int maxEraseCount;

		// Безопасный ввод количества чисел
		System.out.print("Введите общее количество чисел (от 5 до 50000): ");
		while (true) {
			try {
				totalNumbers = scanner.nextInt();
				if (totalNumbers >= 5 && totalNumbers <= 50000) {
					break;
				} else {
					System.out.print(
						"Некорректный ввод. Пожалуйста, введите число от 5 до 50000: "
					);
				}
			} catch (InputMismatchException e) {
				System.out.print(
					"Некорректный ввод. Пожалуйста, введите число от 5 до 50000: "
				);
				scanner.next(); // Consume the invalid input
			}
		}

		// Безопасный ввод максимально стираемых чисел
		System.out.print(
			"Введите максимальное количество чисел, которые можно стереть за ход (от 4 до 100): "
		);
		while (true) {
			try {
				maxEraseCount = scanner.nextInt();
				if (maxEraseCount >= 4 && maxEraseCount <= 100) {
					break;
				} else {
					System.out.print(
						"Некорректный ввод. Пожалуйста, введите число от 4 до 100: "
					);
				}
			} catch (InputMismatchException e) {
				System.out.print(
					"Некорректный ввод. Пожалуйста, введите число от 4 до 100: "
				);
				scanner.next(); // Consume the invalid input
			}
		}

		// Ввод последовательности чисел
		Vector<Integer> numbers = new Vector<>(totalNumbers);
		System.out.println(
			"Введите " + totalNumbers + " целых чисел, разделенных пробелами: "
		);
		for (int i = 0; i < totalNumbers; ++i) {
			while (true) {
				try {
					numbers.add(scanner.nextInt());
					break;
				} catch (InputMismatchException e) {
					System.out.print(
						"Некорректный ввод. Пожалуйста, введите целое число: "
					);
					scanner.next(); // Consume the invalid input
				}
			}
		}

		// Вычисление победителя
		int winner = calculateWinner(numbers, maxEraseCount);

		// Вывод результата
		System.out.println(winner);
	}
}
