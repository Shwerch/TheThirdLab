import java.util.Random;
import java.util.concurrent.ThreadLocalRandom;

public class Utils {

	// Модульное умножение (a * b) % mod с защитой от переполнения
	// This implementation uses the "Russian peasant multiplication" method
	// to avoid overflow when a*b exceeds Long.MAX_VALUE.
	public static long modMul(long a, long b, long mod) {
		long res = 0;
		a %= mod;
		while (b > 0) {
			if ((b & 1) == 1) {
				// Check for overflow before adding
				if (res > mod - a) { // res + a would overflow mod
					res = (res + a) - mod; // Equivalent to (res + a) % mod for positive numbers
				} else {
					res = res + a;
				}
			}
			// Check for overflow before doubling a
			if (a > mod - a) { // a * 2 would overflow mod
				a = (a + a) - mod; // Equivalent to (a * 2) % mod for positive numbers
			} else {
				a = a * 2;
			}
			b >>= 1;
		}
		return res;
	}

	// Модульное возведение в степень: (base^exp) % mod
	public static long modPow(long base, long exp, long mod) {
		long res = 1;
		base %= mod;
		while (exp > 0) {
			if (exp % 2 == 1) res = modMul(res, base, mod);
			base = modMul(base, base, mod);
			exp /= 2;
		}
		return res;
	}

	// Генерация случайного uint64_t указанной битовой длины (до 64 бит)
	public static long generateRandomUint64(int bits) {
		if (bits <= 0 || bits > 64) {
			throw new IllegalArgumentException(
				"Bit count must be between 1 and 64."
			);
		}

		long randomVal = ThreadLocalRandom.current().nextLong();

		// Create a mask to keep only the desired number of bits
		long mask;
		if (bits == 64) {
			mask = -1L; // All bits set
		} else {
			mask = (1L << bits) - 1;
		}
		randomVal &= mask;

		// Set the most significant bit to ensure the number has the specified bit length
		if (bits > 0) {
			long msbMask = 1L << (bits - 1);
			randomVal |= msbMask;
		}
		return randomVal;
	}

	// Генерация случайного uint64_t в диапазоне [min, max]
	public static long generateRandomInRange(long min, long max) {
		if (min > max) {
			throw new IllegalArgumentException(
				"min cannot be greater than max."
			);
		}
		return ThreadLocalRandom.current().nextLong(min, max + 1); // max + 1 because nextLong is exclusive on upper bound
	}

	// Переносимая Java-реализация для определения битовой длины.
	public static int bit_length(long n) {
		if (n == 0) return 0;
		return 64 - Long.numberOfLeadingZeros(n);
	}
}
