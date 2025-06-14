#include "library.h"

// Вычисляет победителя игры
int calculateWinner(const std::vector<int>& numbers, int maxEraseCount) {
    // Создание таблицы для мемоизации результатов, инициализация с -1
    std::vector<std::vector<GameResult>> memo(numbers.size() + 1, std::vector<GameResult>(2, {-1, -1}));
    
    // Решение игры, начиная с хода Павла
    GameResult finalResult = solveGame(numbers, maxEraseCount, true, memo);

    // Определение победителя
    if (finalResult.pavelScore > finalResult.vikaScore) {
        return 1; // Павел победил
    } else {
        return 0; // Вика победила
    }
}

// Рекурсивная функция для определения оптимального хода
GameResult solveGame(const std::vector<int>& currentNumbers, int maxEraseCount, bool isPavelTurn, std::vector<std::vector<GameResult>>& memo) {
    // Базовый случай: если чисел не осталось, игра окончена
    if (currentNumbers.empty()) {
        return {0, 0};
    }

    // Проверка мемоизации
    int currentSize = currentNumbers.size();
    int turnIndex = isPavelTurn ? 0 : 1;
    if (memo[currentSize][turnIndex].pavelScore != -1) {
        return memo[currentSize][turnIndex];
    }

    long long bestPavelScore = (isPavelTurn ? -2e18 : 2e18); // Инициализация для Павла
    long long bestVikaScore = (isPavelTurn ? 2e18 : -2e18); // Инициализация для Вики

    // Игроки действуют оптимально, поэтому выбирают ход, который максимизирует/минимизирует их счет
    for (int i = 1; i <= std::min(maxEraseCount, currentSize); ++i) {
        // Вычисление суммы стираемых чисел
        long long currentSum = 0;
        for (int j = 0; j < i; ++j) {
            currentSum += currentNumbers[j];
        }

        // Создание новой последовательности чисел после хода
        std::vector<int> nextNumbers;
        nextNumbers.reserve(currentSize - i); // Резервирование памяти
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
