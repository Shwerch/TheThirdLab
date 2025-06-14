#include "modules/prime_numbers/gost_generator.hpp"
#include "modules/prime_numbers/primality_tests.hpp"
#include "modules/prime_numbers/utils.hpp"

#include <limits>
#include <stdexcept>

uint64_t generateGostPrimeQ(int q_bits) {
	while (true) {
		uint64_t q = generateRandomUint64(q_bits);
		q |= 1;

		if (isProbablePrimeMillerRabin(q, 15)) {
			return q;
		}
	}
}

uint64_t generateGostPrimeP(int p_bits, int q_bits, int &rejected_count) {
	if (p_bits != 64) {
		throw std::invalid_argument("This implementation is tuned for p_bits=64.");
	}

	rejected_count = 0;
	while (true) {
		uint64_t q = generateGostPrimeQ(q_bits);
		uint64_t two_q = 2 * q;
		if (two_q == 0)
			continue;

		uint64_t seed = generateRandomUint64(p_bits);
		uint64_t p0 = seed - (seed % two_q) + 1;

		// ИСПРАВЛЕНИЕ: Замена __builtin_clzll на переносимую функцию bit_length
		if (bit_length(p0) < p_bits) {
			p0 += two_q;
		}

		for (uint64_t N = 0; N < 200000; ++N) {
			uint64_t p_candidate = p0;
			uint64_t term_to_add = 0;

			if (N > 0) {
				if (two_q > std::numeric_limits<uint64_t>::max() / N) {
					break;
				}
				term_to_add = N * two_q;
			}

			if (term_to_add > std::numeric_limits<uint64_t>::max() - p_candidate) {
				break;
			}
			p_candidate += term_to_add;

			if (isProbablePrimeMillerRabin(p_candidate, 10)) {
				return p_candidate;
			}
			rejected_count++;
		}
	}
}
