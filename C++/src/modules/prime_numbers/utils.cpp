#include "modules/prime_numbers/utils.hpp"

#include <limits>
#include <random>
#include <stdexcept>

uint64_t modMul(uint64_t a, uint64_t b, uint64_t mod) {
	uint64_t res = 0;
	a %= mod;
	while (b > 0) {
		if (b & 1) {
			uint64_t temp_res = res + a;
			if (temp_res < res) { // Проверка на переполнение сложения
				res = (temp_res % mod + (std::numeric_limits<uint64_t>::max() % mod) + 1) % mod;
			} else {
				res = temp_res % mod;
			}
		}
		uint64_t temp_a = a * 2;
		if (temp_a < a) { // Проверка на переполнение умножения на 2
			a = (temp_a % mod + (std::numeric_limits<uint64_t>::max() % mod) + 1) % mod;
		} else {
			a = temp_a % mod;
		}

		b >>= 1;
	}
	return res;
}

uint64_t modPow(uint64_t base, uint64_t exp, uint64_t mod) {
	uint64_t res = 1;
	base %= mod;
	while (exp > 0) {
		if (exp % 2 == 1)
			res = modMul(res, base, mod);
		base = modMul(base, base, mod);
		exp /= 2;
	}
	return res;
}

uint64_t generateRandomUint64(int bits) {
	if (bits <= 0 || bits > 64) {
		throw std::invalid_argument("Bit count must be between 1 and 64.");
	}

	std::random_device rd;
	std::mt19937_64 gen(rd());

	uint64_t random_val = gen();

	uint64_t mask;
	if (bits == 64) {
		mask = std::numeric_limits<uint64_t>::max();
	} else {
		mask = (1ULL << bits) - 1;
	}
	random_val &= mask;

	// Устанавливаем старший бит, чтобы гарантировать длину числа
	if (bits > 0) {
		uint64_t msb_mask = 1ULL << (bits - 1);
		random_val |= msb_mask;
	}

	return random_val;
}

uint64_t generateRandomInRange(uint64_t min, uint64_t max) {
	if (min > max) {
		throw std::invalid_argument("min cannot be greater than max.");
	}
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<uint64_t> distrib(min, max);
	return distrib(gen);
}

int bit_length(uint64_t n) {
	if (n == 0)
		return 0;
	int length = 0;
	if (n >= (1ULL << 32)) {
		length += 32;
		n >>= 32;
	}
	if (n >= (1ULL << 16)) {
		length += 16;
		n >>= 16;
	}
	if (n >= (1ULL << 8)) {
		length += 8;
		n >>= 8;
	}
	if (n >= (1ULL << 4)) {
		length += 4;
		n >>= 4;
	}
	if (n >= (1ULL << 2)) {
		length += 2;
		n >>= 2;
	}
	if (n >= (1ULL << 1)) {
		length += 1;
	}
	if (n > 0) {
		length += 1;
	}
	return length;
}
