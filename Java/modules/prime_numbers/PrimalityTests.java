import java.util.Collections;
import java.util.Random;
import java.util.Vector;

public class PrimalityTests {

	private static final Random random = new Random();

	// Решето Эратосфена для малых простых чисел
	public static Vector<Integer> sieveOfEratosthenes(int limit) {
		boolean[] is_prime = new boolean[limit + 1];
		java.util.Arrays.fill(is_prime, true);
		is_prime[0] = is_prime[1] = false;

		for (int p = 2; p * p <= limit; ++p) {
			if (is_prime[p]) {
				for (int i = p * p; i <= limit; i += p) is_prime[i] = false;
			}
		}

		Vector<Integer> primes = new Vector<>();
		for (int p = 2; p <= limit; ++p) {
			if (is_prime[p]) {
				primes.add(p);
			}
		}
		return primes;
	}

	// Вероятностный тест на простоту Миллера-Рабина для uint64_t (long in Java)
	public static boolean isProbablePrimeMillerRabin(long n, int iterations) {
		if (n < 2) return false;
		if (n == 2 || n == 3) return true;
		if (n % 2 == 0 || n % 3 == 0) return false;

		// Use a static list of small primes to pre-filter
		// In Java, this would ideally be a pre-computed constant or a lazily
		// initialized list.
		// For demonstration, regenerating or keeping it simple.
		Vector<Integer> smallPrimes = sieveOfEratosthenes(500);
		for (int p : smallPrimes) {
			if (n == p) return true;
			if (n % p == 0) return false;
		}

		long d = n - 1;
		while (d % 2 == 0) {
			d /= 2;
		}

		for (int i = 0; i < iterations; ++i) {
			long a = Utils.generateRandomInRange(2, n - 2); // Random 'a' in [2, n-2]
			long x = Utils.modPow(a, d, n);
			if (x == 1 || x == n - 1) continue;

			boolean prime_candidate = false;
			long d_temp = d;
			while (d_temp != n - 1) {
				x = Utils.modMul(x, x, n);
				d_temp *= 2;
				if (x == n - 1) {
					prime_candidate = true;
					break;
				}
			}
			if (!prime_candidate) return false;
		}
		return true;
	}

	// Процедура получения простого числа на основе теста Миллера-Рабина
	public static long generatePrimeMillerRabin(
		int bits,
		int[] rejected_count
	) {
		rejected_count[0] = 0;
		while (true) {
			long p = Utils.generateRandomUint64(bits);
			if (p % 2 == 0) p++;
			if (isProbablePrimeMillerRabin(p, 10)) {
				return p;
			}
			rejected_count[0]++;
		}
	}

	// Процедура получения простого числа на основе теста Поклингтона
	public static long generatePrimePocklington(
		int bits,
		int[] rejected_count
	) {
		if (bits < 4) throw new IllegalArgumentException(
			"Pocklington generation needs more bits"
		);
		rejected_count[0] = 0;
		int q_bits = bits / 2 + 1;
		long q = generatePrimeMillerRabin(q_bits, rejected_count); // This will update rejected_count

		long R = 1;
		while (true) {
			long p_candidate = 2 * R * q + 1;

			// Check for overflow or exceeding bit length
			if (
				p_candidate < R ||
				Utils.bit_length(p_candidate) > bits ||
				p_candidate < 0
			) { // p_candidate < 0 if overflow occurred
				q = generatePrimeMillerRabin(q_bits, rejected_count);
				R = 1;
				continue;
			}

			if (isProbablePrimeMillerRabin(p_candidate, 10)) {
				return p_candidate;
			}
			R++;
			rejected_count[0]++;
		}
	}
}
