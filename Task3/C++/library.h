#pragma once // Гарантирует, что файл будет включен только один раз

#include <iostream> // Для ввода/вывода (std::cout, std::cin, std::endl)
#include <string>   // Для работы со строками (std::string)
#include <vector>   // Для использования std::vector
#include <numeric>  // Для std::gcd (начиная с C++17)
#include <cmath>    // Для std::pow

// Структура для представления рационального числа
struct Rational {
    long long numer; // Числитель
    long long denom; // Знаменатель
};

// Вычисляет наибольший общий делитель двух чисел
long long gcd(long long numA, long long numB);

// Сокращает рациональную дробь до несократимого вида
Rational simplifyRational(long long numer, long long denom);

// Вычисляет и печатает сумму ряда
void calculateAndPrintSeriesSum(int valA, int valB);
