#include "../include/gost_generator.h"
#include "../include/primality_tests.h"

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

struct ExperimentResult {
	int id;
	uint64_t p;
	char verification_result;
	int k;
};

void printResultsTable(const std::vector<ExperimentResult> &results) {
	std::cout << "| N" << std::setw(3) << " |" << std::setw(15) << "P" << std::setw(8) << "|"
			  << " Проверка  |"
			  << " K      |\n";

	for (const ExperimentResult &res : results) {
		std::cout << "| " << std::setw(2) << res.id << " | " << std::left << std::setw(20) << res.p
				  << std::right << " |" << std::setw(6) << res.verification_result << std::setw(6)
				  << " |" << std::setw(7) << res.k << " |\n";
	}
	std::cout << std::endl;
}

int main() {
	std::setlocale(LC_ALL, "Russian");

	std::cout << "Таблица простых чисел < 500 (Решето Эратосфена):\n";
	const std::vector<int> small_primes = sieveOfEratosthenes(500);
	for (size_t i = 0; i < small_primes.size(); ++i) {
		std::cout << std::setw(4) << small_primes[i] << ((i + 1) % 15 == 0 ? "\n" : "");
	}
	std::cout << "\n\nВсего малых простых: " << small_primes.size() << "\n\n";

	std::vector<ExperimentResult> results;
	const int p_bits = 64;
	const int q_bits = 32;

	// Вероятность ошибки теста Миллера-Рабина < 0.1. (1/4)^t < 0.1 => t > 1.66. Берем t=2
	const int miller_rabin_iterations = 2;

	std::cout << "Генерация 10 простых чисел по ГОСТ (p: 64 бит, q: 32 бит)\n";
	for (int i = 0; i < 10; i++) {
		int k = 0;
		uint64_t p = generateGostPrimeP(p_bits, q_bits, k);

		bool is_prime_check = isProbablePrimeMillerRabin(p, miller_rabin_iterations);

		results.push_back({i, p, (is_prime_check ? '+' : '-'), k});
	}
	printResultsTable(results);
	results.clear();

	std::cout << "Генерация 10 простых чисел по тесту Миллера\n";
	for (int i = 0; i < 10; i++) {
		int k = 0;
		uint64_t p = generatePrimeMillerRabin(p_bits, k);

		bool is_prime_check = isProbablePrimeMillerRabin(p, miller_rabin_iterations);

		results.push_back({i, p, (is_prime_check ? '+' : '-'), k});
	}
	printResultsTable(results);
	results.clear();

	std::cout << "Генерация 10 простых чисел по тесту Поклингтона\n";
	for (int i = 0; i < 10; i++) {
		int k = 0;
		uint64_t p = generatePrimePocklington(p_bits, k);

		bool is_prime_check = isProbablePrimeMillerRabin(p, miller_rabin_iterations);

		results.push_back({i, p, (is_prime_check ? '+' : '-'), k});
	}
	printResultsTable(results);
	results.clear();

	return 0;
}
