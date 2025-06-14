import java.security.SecureRandom;
import java.util.Random;

public class GostGenerator {

	private static final Random random = new SecureRandom();

	private static long generateGostPrimeQ(int q_bits) {
		while (true) {
			long q = Utils.generateRandomUint64(q_bits);
			q |= 1; // Ensure it's odd
			if (PrimalityTests.isProbablePrimeMillerRabin(q, 15)) {
				return q;
			}
		}
	}

	public static long generateGostPrimeP(
		int p_bits,
		int q_bits,
		int[] rejected_count
	) {
		if (p_bits != 64) {
			throw new IllegalArgumentException(
				"This implementation is tuned for p_bits=64."
			);
		}

		rejected_count[0] = 0; // Using array to pass by reference
		while (true) {
			long q = generateGostPrimeQ(q_bits);
			long two_q = 2 * q;
			if (
				two_q == 0
			) continue; // Should not happen with positive q

			long seed = Utils.generateRandomUint64(p_bits);
			long p0 = seed - (seed % two_q) + 1;

			if (Utils.bit_length(p0) < p_bits) {
				p0 += two_q;
			}

			for (long N = 0; N < 200000; ++N) {
				long p_candidate = p0;
				long term_to_add = 0;

				if (N > 0) {
					// Check for potential overflow before multiplication
					if (two_q > Long.MAX_VALUE / N) {
						break; // Avoid overflow, this path won't lead to a valid p_candidate
					}
					term_to_add = N * two_q;
				}

				if (term_to_add > Long.MAX_VALUE - p_candidate) {
					break; // Avoid overflow
				}
				p_candidate += term_to_add;

				if (
					PrimalityTests.isProbablePrimeMillerRabin(p_candidate, 10)
				) {
					return p_candidate;
				}
				rejected_count[0]++;
			}
		}
	}
}
