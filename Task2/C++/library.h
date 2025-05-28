#pragma once

#include <algorithm>
#include <iomanip> // Для std::setw
#include <iostream>
#include <numeric> // Для std::gcd в C++17, или можно реализовать вручную
#include <random>
#include <string>
#include <vector>

#define PRIME_NUMBERS 10

// Функция для генерации простых чисел с помощью решета Эратосфена
std::vector<bool> sieveEratosthenes(int limit);

// Функция для проверки числа на простоту с использованием теста Миллера-Рабина
bool millerRabinTest(long long num, double errorProb);

// Функция для получения простого числа заданной длины (меньше 500) на основе теста Миллера
long long getPrimeMiller(int limit);

// Функция для получения простого числа заданной длины (меньше 500) на основе теста Поклингтона
long long getPrimePocklington(const std::vector<bool> &primeTable, int limit);

// Функция для генерации простого числа заданной длины (меньше 500) по ГОСТ Р 34.10-94
long long getPrimeGOST(const std::vector<bool> &primeTable, int limit);

// Функция для вычисления модульной степени (a^b % m)
long long power(long long baseVal, long long expVal, long long modVal);

// Функция для вычисления НОД
long long gcd(long long firstNum, long long secondNum);

// Структура для представления простого числа (если понадобится хранить доп. информацию)
struct PrimeInfo {
	long long value;
	// Дополнительные поля, если необходимо
};
