#include "modules/game/game.hpp"

// Рекурсивная функция для определения оптимального хода
GameResult solveGame(const std::vector<int> &currentNumbers, int maxEraseCount, bool isPavelTurn,
					 std::vector<std::vector<GameResult>> &memo) {
	if (currentNumbers.empty()) {
		return {0, 0};
	}

	int currentSize = currentNumbers.size();
	int turnIndex = isPavelTurn ? 0 : 1;
	if (memo[currentSize][turnIndex].pavelScore != -1) {
		return memo[currentSize][turnIndex];
	}

	long long bestPavelScore = (isPavelTurn ? -2e18 : 2e18); // Инициализация для Павла
	long long bestVikaScore = (isPavelTurn ? 2e18 : -2e18);	 // Инициализация для Вики

	// Выбирают ход, который максимизирует/минимизирует их счет
	for (int i = 1; i <= std::min(maxEraseCount, currentSize); ++i) {
		long long currentSum = 0;
		for (int j = 0; j < i; ++j) {
			currentSum += currentNumbers[j];
		}

		std::vector<int> nextNumbers;
		nextNumbers.reserve(currentSize - i);
		for (int j = i; j < currentSize; ++j) {
			nextNumbers.push_back(currentNumbers[j]);
		}

		// Рекурсивный вызов для следующего хода
		GameResult nextResult = solveGame(nextNumbers, maxEraseCount, !isPavelTurn, memo);

		// Обновление счетов
		if (isPavelTurn) {
			long long pavelCandidateScore = currentSum + nextResult.pavelScore;
			long long vikaCandidateScore = nextResult.vikaScore;

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
			long long pavelCandidateScore = nextResult.pavelScore;
			long long vikaCandidateScore = currentSum + nextResult.vikaScore;

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
	memo[currentSize][turnIndex] = {bestPavelScore, bestVikaScore};
	return {bestPavelScore, bestVikaScore};
}

// Вычисляет победителя игры
int calculateWinner(const std::vector<int> &numbers, int maxEraseCount) {
	// Создание таблицы для мемоизации результатов, инициализация с -1
	std::vector<std::vector<GameResult>> memo(numbers.size() + 1,
											  std::vector<GameResult>(2, {-1, -1}));

	// Решение игры, начиная с хода Павла
	GameResult finalResult = solveGame(numbers, maxEraseCount, true, memo);

	// Определение победителя
	if (finalResult.pavelScore > finalResult.vikaScore) {
		return 1; // Павел победил
	} else {
		return 0; // Вика победила
	}
}

void startGame() {
	int totalNumbers;
	int maxEraseCount;

	std::cout << "Введите общее количество чисел (от 5 до 50000): ";
	while (!(std::cin >> totalNumbers) || totalNumbers < 5 || totalNumbers > 50000) {
		std::cout << "Некорректный ввод. Пожалуйста, введите число от 5 до 50000: ";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	std::cout
		<< "Введите максимальное количество чисел, которые можно стереть за ход (от 4 до 100): ";
	while (!(std::cin >> maxEraseCount) || maxEraseCount < 4 || maxEraseCount > 100) {
		std::cout << "Некорректный ввод. Пожалуйста, введите число от 4 до 100: ";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	std::vector<int> numbers(totalNumbers);
	std::cout << "Введите " << totalNumbers << " целых чисел, разделенных пробелами: " << std::endl;
	for (int i = 0; i < totalNumbers; ++i) {
		while (!(std::cin >> numbers[i])) {
			std::cout << "Некорректный ввод. Пожалуйста, введите целое число: ";
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}

	int winner = calculateWinner(numbers, maxEraseCount);

	std::cout << winner << std::endl;
}
