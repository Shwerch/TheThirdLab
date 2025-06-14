#include "modules/prime_numbers/primality_tests.hpp"
#include "modules/prime_numbers/utils.hpp"

#include <stdexcept>
#include <vector>

std::vector<int> sieveOfEratosthenes(int limit) {
	std::vector<bool> is_prime(limit + 1, true);
	is_prime[0] = is_prime[1] = false;
	for (int p = 2; p * p <= limit; ++p) {
		if (is_prime[p]) {
			for (int i = p * p; i <= limit; i += p)
				is_prime[i] = false;
		}
	}
	std::vector<int> primes;
	for (int p = 2; p <= limit; ++p) {
		if (is_prime[p]) {
			primes.push_back(p);
		}
	}
	return primes;
}

bool isProbablePrimeMillerRabin(uint64_t n, int iterations) {
	if (n < 2)
		return false;
	if (n == 2 || n == 3)
		return true;
	if (n % 2 == 0 || n % 3 == 0)
		return false;

	static const std::vector<int> smallPrimes = sieveOfEratosthenes(500);
	for (int p : smallPrimes) {
		// ИСПРАВЛЕНИЕ: приведение типа для корректного сравнения
		if (n == static_cast<uint64_t>(p))
			return true;
		if (n % static_cast<uint64_t>(p) == 0)
			return false;
	}

	uint64_t d = n - 1;
	while (d % 2 == 0) {
		d /= 2;
	}

	for (int i = 0; i < iterations; ++i) {
		uint64_t a = generateRandomInRange(2, n - 2);
		uint64_t x = modPow(a, d, n);
		if (x == 1 || x == n - 1)
			continue;

		bool prime_candidate = false;
		uint64_t d_temp = d;
		while (d_temp != n - 1) {
			x = modMul(x, x, n);
			d_temp *= 2;
			if (x == n - 1) {
				prime_candidate = true;
				break;
			}
		}
		if (!prime_candidate)
			return false;
	}
	return true;
}

uint64_t generatePrimeMillerRabin(int bits, int &rejected_count) {
	rejected_count = 0;
	while (true) {
		uint64_t p = generateRandomUint64(bits);
		if (p % 2 == 0)
			p++;

		if (isProbablePrimeMillerRabin(p, 10)) {
			return p;
		}
		rejected_count++;
	}
}

uint64_t generatePrimePocklington(int bits, int &rejected_count) {
	if (bits < 4)
		throw std::invalid_argument("Pocklington generation needs more bits");
	rejected_count = 0;
	int q_bits = bits / 2 + 1;
	uint64_t q = generatePrimeMillerRabin(q_bits, rejected_count);

	uint64_t R = 1;
	while (true) {
		uint64_t p_candidate = 2 * R * q + 1;

		if (p_candidate < R || bit_length(p_candidate) > bits) {
			q = generatePrimeMillerRabin(q_bits, rejected_count);
			R = 1;
			continue;
		}

		if (isProbablePrimeMillerRabin(p_candidate, 10)) {
			return p_candidate;
		}
		R++;
		rejected_count++;
	}
}
