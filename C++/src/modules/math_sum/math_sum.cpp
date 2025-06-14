#include "modules/math_sum/math_sum.hpp"

#include <cmath>
#include <ios>
#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <vector>

long long pow(int base, int exp) {
	long long result = 1;
	for (int i = 0; i < exp; ++i)
		result *= base;
	return result;
}

long long gcd(long long numA, long long numB) { return std::gcd(numA, numB); }

Rational simplifyRational(long long numer, long long denom) {
	if (denom == 0) {
		return {numer, denom};
	}
	long long commonDivisor = gcd(std::abs(numer), std::abs(denom));
	long long simplifiedNumer = numer / commonDivisor;
	long long simplifiedDenom = denom / commonDivisor;

	if (simplifiedDenom < 0) {
		simplifiedNumer = -simplifiedNumer;
		simplifiedDenom = -simplifiedDenom;
	}
	return {simplifiedNumer, simplifiedDenom};
}

void calculateAndPrintSeriesSum(int valA, int valB) {
	if (valB == 1) {
		std::cout << "Ряд расходится: infinity" << std::endl;
		return;
	}

	long long numerSum = 0;
	long long denomSum = 1;

	std::vector<long long> numCoeffs;

	switch (valA) {
	case 1:
		numerSum = valB;
		denomSum = pow(valB - 1, 2);
		break;
	case 2:
		numerSum = (long long)valB * (valB + 1);
		denomSum = pow(valB - 1, 3);
		break;
	case 3:
		// numCoeffs = {1, 4, 1}; // Коэффициенты для (b^2+4b+1)
		numerSum = (long long)valB * (pow(valB, 2) + 4 * valB + 1);
		denomSum = pow(valB - 1, 4);
		break;
	case 4:
		// numCoeffs = {1, 11, 11, 1}; // Коэффициенты для (b^3+11b^2+11b+1)
		numerSum = (long long)valB * (pow(valB, 3) + 11 * pow(valB, 2) + 11 * valB + 1);
		denomSum = pow(valB - 1, 5);
		break;
	case 5:
		// numCoeffs = {1, 26, 66, 26, 1};
		numerSum = (long long)valB *
				   (pow(valB, 4) + 26 * pow(valB, 3) + 66 * pow(valB, 2) + 26 * valB + 1);
		denomSum = pow(valB - 1, 6);
		break;
	case 6:
		// numCoeffs = {1, 57, 302, 302, 57, 1};
		numerSum = (long long)valB * (pow(valB, 5) + 57 * pow(valB, 4) + 302 * pow(valB, 3) +
									  302 * pow(valB, 2) + 57 * valB + 1);
		denomSum = pow(valB - 1, 7);
		break;
	case 7:
		// numCoeffs = {1, 120, 1191, 2416, 1191, 120, 1};
		numerSum = (long long)valB * (pow(valB, 6) + 120 * pow(valB, 5) + 1191 * pow(valB, 4) +
									  2416 * pow(valB, 3) + 1191 * pow(valB, 2) + 120 * valB + 1);
		denomSum = pow(valB - 1, 8);
		break;
	case 8:
		// numCoeffs = {1, 247, 4293, 15619, 15619, 4293, 247, 1};
		numerSum = (long long)valB *
				   (pow(valB, 7) + 247 * pow(valB, 6) + 4293 * pow(valB, 5) + 15619 * pow(valB, 4) +
					15619 * pow(valB, 3) + 4293 * pow(valB, 2) + 247 * valB + 1);
		denomSum = pow(valB - 1, 9);
		break;
	case 9:
		// numCoeffs = {1, 502, 14608, 88234, 156190, 88234, 14608, 502, 1};
		numerSum = (long long)valB * (pow(valB, 8) + 502 * pow(valB, 7) + 14608 * pow(valB, 6) +
									  88234 * pow(valB, 5) + 156190 * pow(valB, 4) +
									  88234 * pow(valB, 3) + 14608 * pow(valB, 2) + 502 * valB + 1);
		denomSum = pow(valB - 1, 10);
		break;
	case 10:
		// numCoeffs = {1, 1013, 47840, 455192, 1310356, 1310356, 455192, 47840, 1013, 1};
		numerSum = (long long)valB *
				   (pow(valB, 9) + 1013 * pow(valB, 8) + 47840 * pow(valB, 7) +
					455192 * pow(valB, 6) + 1310356 * pow(valB, 5) + 1310356 * pow(valB, 4) +
					455192 * pow(valB, 3) + 47840 * pow(valB, 2) + 1013 * valB + 1);
		denomSum = pow(valB - 1, 11);
		break;
	default:
		// Этот случай не должен быть достигнут из-за проверки ввода
		std::cout << "Неизвестное значение 'a'." << std::endl;
		return;
	}

	Rational resultFrac = simplifyRational(numerSum, denomSum);

	std::cout << "Сумма ряда: " << resultFrac.numer << "/" << resultFrac.denom << " ("
			  << resultFrac.numer / static_cast<double>(resultFrac.denom) << ")" << std::endl;
}
void calculateMathSum() {
	// Выводим приветствие
	std::cout << std::string(30, '-') << std::endl;
	std::cout << "Программа для вычисления суммы ряда" << std::endl;
	std::cout << std::string(30, '-') << std::endl;

	int valA = 0;
	int valB = 0;

	while (true) {
		std::cout << "Введите целое число 'a' (от 1 до 10): ";
		if (!(std::cin >> valA) || valA < 1 || valA > 10) {
			std::cout << "Некорректный ввод. Пожалуйста, введите число от 1 до 10." << std::endl;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		} else {
			break;
		}
	}

	// Запрашиваем ввод 'b' и проверяем его
	while (true) {
		std::cout << "Введите целое число 'b' (от 1 до 10): ";
		if (!(std::cin >> valB) || valB < 1 || valB > 10) {
			std::cout << "Некорректный ввод. Пожалуйста, введите число от 1 до 10." << std::endl;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		} else {
			break;
		}
	}

	calculateAndPrintSeriesSum(valA, valB);
}
