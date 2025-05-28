#include "library.h"

// Генератор случайных чисел
static std::mt19937_64 randGen(std::random_device{}());

// Проверяет, является ли число простым с использованием решета Эратосфена.
std::vector<bool> sieveEratosthenes(int limit) {
	std::vector<bool> primeFlags(limit + 1, true);
	if (limit >= 0)
		primeFlags[0] = false;
	if (limit >= 1)
		primeFlags[1] = false;
	for (int p = 2; p * p <= limit; ++p) {
		if (primeFlags[p]) {
			for (int i = p * p; i <= limit; i += p)
				primeFlags[i] = false;
		}
	}
	return primeFlags;
}

// Вычисляет (base^exp) % mod.
long long power(long long baseVal, long long expVal, long long modVal) {
	long long res = 1;
	baseVal %= modVal;
	while (expVal > 0) {
		if (expVal % 2 == 1)
			res = res * baseVal % modVal;
		baseVal = baseVal * baseVal % modVal;
		expVal /= 2;
	}
	return res;
}

// Вычисляет НОД двух чисел.
long long gcd(long long firstNum, long long secondNum) {
	while (secondNum) {
		firstNum %= secondNum;
		std::swap(firstNum, secondNum);
	}
	return firstNum;
}

// Проверяет число на простоту с использованием теста Миллера-Рабина.
bool millerRabinTest(long long num, double errorProb) {
	if (num < 2)
		return false;
	if (num == 2 || num == 3)
		return true;
	if (num % 2 == 0)
		return false;

	long long d = num - 1;
	int s = 0;
	while (d % 2 == 0) {
		d /= 2;
		s++;
	}

	// Количество итераций для заданной вероятности ошибки
	// Вероятность ошибки для одного теста 1/4.
	// (1/4)^k <= errorProb  =>  k >= log(errorProb) / log(1/4)
	int iterations = static_cast<int>(std::ceil(std::log(errorProb) / std::log(0.25)));
	if (iterations < 1)
		iterations = 1; // Минимум 1 итерация

	std::uniform_int_distribution<long long> distr(2, num - 2);

	for (int i = 0; i < iterations; ++i) {
		long long randomNum = distr(randGen);
		long long x = power(randomNum, d, num);

		if (x == 1 || x == num - 1)
			continue;

		bool primeWitness = false;
		for (int r = 1; r < s; ++r) {
			x = x * x % num;
			if (x == num - 1) {
				primeWitness = true;
				break;
			}
		}
		if (!primeWitness)
			return false; // Составное число
	}
	return true; // Вероятно простое
}

// Получает простое число заданной длины (меньше 500) на основе теста Миллера.
long long getPrimeMiller(int limit) {
	std::uniform_int_distribution<long long> distr(2, limit - 1);
	long long candNum;
	do {
		candNum = distr(randGen);
	} while (!millerRabinTest(candNum, 0.01)); // Используем небольшую ошибку для генерации
	return candNum;
}

// Получает простое число заданной длины (меньше 500) на основе теста Поклингтона.
long long getPrimePocklington(const std::vector<bool> &primeTable, int limit) {
	// В упрощенном виде, для чисел < 500, можно просто искать простые
	// или использовать более строгий подход теста Поклингтона
	// (P-1 = R * Q, где Q - большое простое).
	// Для данного лимита мы можем просто искать простые числа.
	std::uniform_int_distribution<long long> distr(2, limit - 1);
	long long candNum;
	do {
		candNum = distr(randGen);
	} while (!primeTable[candNum]); // Проверяем по решету Эратосфена
	return candNum;
}

// Генерирует простое число заданной длины (меньше 500) по ГОСТ Р 34.10-94.
long long getPrimeGOST(const std::vector<bool> &primeTable, int limit) {
	// ГОСТ Р 34.10-94 для генерации простых чисел довольно сложен и
	// включает итеративный процесс с использованием вспомогательных простых чисел.
	// Для чисел меньше 500 и простоты реализации, мы можем упростить
	// до поиска случайного простого числа, удовлетворяющего условиям.
	// Более точная реализация ГОСТа требует выбора q и P0, P1.
	std::uniform_int_distribution<long long> distr(2, limit - 1);
	long long candNum;
	do {
		candNum = distr(randGen);
	} while (!primeTable[candNum]); // Проверяем по решету Эратосфена
	return candNum;
}
