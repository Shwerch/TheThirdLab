#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>

static uint64_t ipow_u64(uint32_t base, uint32_t exp) {
	uint64_t r = 1;
	for (uint32_t i = 0; i < exp; ++i) {
		r *= base;
	}
	return r;
}

// Вычисление чисел Стирлинга второго рода S(n,k) для фиксированного n
static std::vector<uint64_t> stirling2_row(uint32_t n) {
	std::vector<std::vector<uint64_t>> S(n + 1, std::vector<uint64_t>(n + 1, 0));
	S.at(0).at(0) = 1;
	for (uint64_t i = 1; i <= n; ++i) {
		for (uint64_t k = 1; k <= i; ++k) {
			// рекуррентное соотношение: S(i,k) = k*S(i-1,k) + S(i-1,k-1)
			S.at(i).at(k) = S.at(i - 1).at(k - 1) + k * S.at(i - 1).at(k);
		}
	}
	std::vector<uint64_t> row(n + 1);
	for (uint64_t k = 0; k <= n; ++k)
		row.at(k) = S.at(n).at(k);
	return row;
}

// Используются стирлинговые числа второго рода
// (они переводят степенные суммы в комбинацию падающих факториалов)
void calculateMathSum() {
	std::cout << "Введите два числа, разделяя их пробелом: ";
	uint32_t a, b;
	if (!(std::cin >> a >> b))
		return;

	if (a < 1 || a > 10 || b < 1 || b > 10) {
		std::cerr << "Error: a and b must be integers in [1,10]\n";
		return;
	}

	if (b == 1u) {
		std::cout << "infinity\n";
		return;
	}

	// Подготовка: числа Стирлинга и факториалы
	std::vector<uint64_t> Srow = stirling2_row(a); // Srow[m] = S(a,m)
	std::vector<uint64_t> fact(a + 1, 1);
	for (uint64_t i = 1; i <= a; ++i) {
		fact.at(i) = fact.at(i - 1) * i;
	}
	// Приведение суммы к единому знаменателю (b-1)^(a+1)
	// Формула: sum_{n>=1} n^a / b^n = b * sum_{m=0..a} S(a,m) m! / (b-1)^{m+1}
	// общий знаменатель Q = (b-1)^{a+1}; числитель P = b * sum_{m=1..a} S(a,m) m! * (b-1)^{a-m}
	uint64_t Q = ipow_u64((uint64_t)(b - 1u), a + 1u);
	uint64_t P = 0;

	for (uint32_t m = 1; m <= a; ++m) {
		// term = b * S(a,m) * m! * (b-1)^(a-m)
		uint64_t term = (uint64_t)b;
		term *= Srow.at(m);
		term *= fact.at(m);
		term *= ipow_u64((uint64_t)(b - 1u), a - m);
		P += term;
	}

	// Сокращение дроби
	uint64_t g = std::gcd(P, Q);
	P /= g;
	Q /= g;

	std::cout << P << "/" << Q << "\n";
}
