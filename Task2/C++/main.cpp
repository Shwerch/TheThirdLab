#include "library.h"

int main() {
	std::vector<bool> pTable = sieveEratosthenes(500);

	// Вектор для хранения сгенерированных простых чисел
	std::vector<long long> genPrimes;

	// Генерация простых чисел с использованием различных тестов
	std::cout << "Генерация " << PRIME_NUMBERS << " простых чисел:" << std::endl;
	for (int i = 0; i < PRIME_NUMBERS; ++i) {
		long long primeNum;
		if (i % 3 == 0) {
			// Используем тест Миллера
			primeNum = getPrimeMiller(500);
			std::cout << "  Сгенерировано число " << primeNum << " с помощью теста Миллера."
					  << std::endl;
		} else if (i % 3 == 1) {
			// Используем тест Поклингтона
			primeNum = getPrimePocklington(pTable, 500);
			std::cout << "  Сгенерировано число " << primeNum << " с помощью теста Поклингтона."
					  << std::endl;
		} else {
			// Используем ГОСТ Р 34.10-94
			primeNum = getPrimeGOST(pTable, 500);
			std::cout << "  Сгенерировано число " << primeNum << " с помощью ГОСТ Р 34.10-94."
					  << std::endl;
		}
		genPrimes.push_back(primeNum);
	}
	std::cout << std::endl;

	// Таблица результатов
	std::cout << "№" << std::string(7, ' ') << "P" << std::string(7, ' ')
			  << "Test" << std::string(3, ' ') << "k" << std::endl;
	std::cout << std::string(50, '-') << std::endl;

	// Проверка каждого сгенерированного числа и подсчет k
	for (int i = 0; i < PRIME_NUMBERS; ++i) {
		long long currentPrime = genPrimes[i];
		bool isProbablePrime = millerRabinTest(currentPrime, 0.1);

		// Генерация отклоненных чисел (для демонстрации) и их проверка
		int rejectedCount = 0;
		std::vector<long long> rejectedNums;
		// Генерируем 5 случайных чисел, меньших 500, для "отклонения"
		for (int j = 0; j < 5; ++j) {
			long long rejectedNum = rand() % 499 + 2; // Числа от 2 до 500-1
			// Если число не простое (по решету Эратосфена)
			if (!pTable[rejectedNum]) {
				rejectedNums.push_back(rejectedNum);
				if (millerRabinTest(rejectedNum, 0.1)) {
					rejectedCount++;
				}
			}
		}

		std::cout << std::setw(2) << i + 1 << std::string(6, ' ') << std::setw(3) << currentPrime
				  << std::string(5, ' ') << (isProbablePrime ? "+" : "-") << std::string(6, ' ')
				  << rejectedCount << std::endl;
	}
	std::cout << std::endl;

	return 0;
}
