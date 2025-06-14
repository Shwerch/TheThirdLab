#pragma once

#include <cstdint>

// Генерирует простое число p (до 64 бит) по алгоритму ГОСТ Р 34.10-94
uint64_t generateGostPrimeP(int p_bits, int q_bits, int &rejected_count);
