#pragma once

#include <cstdint>

// Модульное умножение (a * b) % mod с защитой от переполнения
uint64_t modMul(uint64_t a, uint64_t b, uint64_t mod);

// Модульное возведение в степень: (base^exp) % mod
uint64_t modPow(uint64_t base, uint64_t exp, uint64_t mod);

// Генерация случайного uint64_t указанной битовой длины (до 64 бит)
uint64_t generateRandomUint64(int bits);

// Генерация случайного uint64_t в диапазоне [min, max]
uint64_t generateRandomInRange(uint64_t min, uint64_t max);

int bit_length(uint64_t n);
