#pragma once

#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <vector>

// Структура для хранения результатов игры
struct GameResult {
	long long pavelScore;
	long long vikaScore;
};

void startGame();
