#include <cmath>
#include <iostream>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

struct Rational {
	long long numer;
	long long denom;
};

long long integer_pow(int base, int exp) {
	if (exp < 0)
		return 0;
	long long result = 1;
	for (int i = 0; i < exp; ++i) {
		result *= base;
	}
	return result;
}

Rational simplifyRational(Rational r) {
	if (r.denom == 0) {
		return r;
	}
	long long commonDivisor = std::gcd(std::abs(r.numer), std::abs(r.denom));
	r.numer /= commonDivisor;
	r.denom /= commonDivisor;

	if (r.denom < 0) {
		r.numer = -r.numer;
		r.denom = -r.denom;
	}
	return r;
}

std::vector<long long> generateEulerianNumbers(int n) {
	if (n < 1)
		return {};
	if (n == 1)
		return {1};

	std::vector<long long> prev_coeffs = {1};
	for (int i = 2; i <= n; ++i) {
		std::vector<long long> current_coeffs(i, 0);
		for (int k = 0; k < i; ++k) {
			long long term1 = (k > 0) ? (long long)(k)*prev_coeffs[k - 1] : 0;
			long long term2 = (k < i - 1) ? (long long)(i - k) * prev_coeffs[k] : 0;
			current_coeffs[k] = term1 + term2;
		}
		prev_coeffs = std::move(current_coeffs);
	}
	std::vector<std::vector<long long>> A(n + 1, std::vector<long long>(n + 1, 0));
	A[0][0] = 1;
	for (int i = 1; i <= n; ++i) {
		for (int k = 1; k <= i; ++k) {
			A[i][k] = (long long)(k)*A[i - 1][k] + (long long)(i - k + 1) * A[i - 1][k - 1];
		}
	}
	return {A[n].begin() + 1, A[n].end()};
}

long long evaluatePolynomial(const std::vector<long long> &coeffs, int x) {
	long long result = 0;
	for (int i = coeffs.size() - 1; i >= 0; --i) {
		result = result * x + coeffs[i];
	}
	return result;
}

void calculateAndPrintSeriesSum(int valA, int valB) {
	if (valB == 1) {
		std::cout << "Ряд расходится: infinity" << std::endl;
		return;
	}

	auto eulerian_coeffs = generateEulerianNumbers(valA);
	long long numerator_poly_val = evaluatePolynomial(eulerian_coeffs, valB);

	Rational frac{(long long)valB * numerator_poly_val, integer_pow(valB - 1, valA + 1)};

	Rational resultFrac = simplifyRational(frac);

	std::cout << "Сумма ряда: " << resultFrac.numer << "/" << resultFrac.denom << " ("
			  << static_cast<double>(resultFrac.numer) / resultFrac.denom << ")" << std::endl;
}

template <typename T> T getValidatedInput(const std::string &prompt, T min, T max) {
	T value;
	while (true) {
		std::cout << prompt;
		if (std::cin >> value && value >= min && value <= max) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			return value;
		}
		std::cout << "Некорректный ввод. Пожалуйста, введите целое число от " << min << " до "
				  << max << "." << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

void calculateMathSum() {
	std::cout << std::string(30, '-') << std::endl;
	std::cout << "Программа для вычисления суммы ряда" << std::endl;
	std::cout << std::string(30, '-') << std::endl;

	int valA = getValidatedInput<int>("Введите целое число 'a' (от 1 до 10): ", 1, 10);
	int valB = getValidatedInput<int>("Введите целое число 'b' (от 1 до 10): ", 1, 10);

	calculateAndPrintSeriesSum(valA, valB);
}
