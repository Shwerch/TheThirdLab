#pragma once

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <limits>
#include <string>

// Структура для хранения результатов игры
struct GameResult {
    long long pavelScore;
    long long vikaScore;
};

// Вычисляет победителя игры
int calculateWinner(const std::vector<int>& numbers, int maxEraseCount);

// Рекурсивная функция для определения оптимального хода
GameResult solveGame(const std::vector<int>& currentNumbers, int maxEraseCount, bool isPavelTurn, std::vector<std::vector<GameResult>>& memo);
