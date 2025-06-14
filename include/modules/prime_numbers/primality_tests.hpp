#pragma once

#include <cstdint>
#include <vector>

// Решето Эратосфена для малых простых чисел
std::vector<int> sieveOfEratosthenes(int limit);

// Вероятностный тест на простоту Миллера-Рабина для uint64_t
bool isProbablePrimeMillerRabin(uint64_t n, int iterations);

// Процедура получения простого числа на основе теста Миллера-Рабина
uint64_t generatePrimeMillerRabin(int bits, int &rejected_count);

// Процедура получения простого числа на основе теста Поклингтона
uint64_t generatePrimePocklington(int bits, int &rejected_count);
