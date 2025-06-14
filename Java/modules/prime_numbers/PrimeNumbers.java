import java.text.DecimalFormat;
import java.util.Formatter;
import java.util.Locale;
import java.util.Vector;

class ExperimentResult {

	public int id;
	public long p;
	public char verification_result;
	public int k;

	public ExperimentResult(int id, long p, char verification_result, int k) {
		this.id = id;
		this.p = p;
		this.verification_result = verification_result;
		this.k = k;
	}
}

public class PrimeNumbers {

	private static void printResultsTable(
		final Vector<ExperimentResult> results
	) {
		System.out.println(
			"| N  |" + String.format("%-23s", "P") + "| Проверка  | K      |"
		);
		for (final ExperimentResult res : results) {
			System.out.printf(
				Locale.US,
				"| %-2d | %-21d |     %c     | %-7d |\n",
				res.id,
				res.p,
				res.verification_result,
				res.k
			);
		}
		System.out.println();
	}

	public static void calculatePrimeNumbers() {
		// In Java, locale for output formatting is typically handled by DecimalFormat
		// or String.format.
		// setlocale(LC_ALL, "Russian") is not directly translatable for console output
		// in the same way.

		System.out.println(
			"Таблица простых чисел < 500 (Решето Эратосфена):\n"
		);
		final Vector<Integer> small_primes = PrimalityTests.sieveOfEratosthenes(
			500
		);
		for (int i = 0; i < small_primes.size(); ++i) {
			System.out.printf(Locale.US, "%4d", small_primes.get(i));
			if ((i + 1) % 15 == 0) {
				System.out.println();
			}
		}
		System.out.println(
			"\n\nВсего малых простых: " + small_primes.size() + "\n\n"
		);

		Vector<ExperimentResult> results = new Vector<>();
		final int p_bits = 64;
		final int q_bits = 32;

		// Вероятность ошибки теста Миллера-Рабина < 0.1. (1/4)^t < 0.1 => t > 1.66.
		// Берем t=2
		final int miller_rabin_iterations = 2;

		System.out.println(
			"Генерация 10 простых чисел по ГОСТ (p: 64 бит, q: 32 бит)\n"
		);
		for (int i = 0; i < 10; i++) {
			int[] k = { 0 }; // Using array to pass by reference
			long p = GostGenerator.generateGostPrimeP(p_bits, q_bits, k);
			boolean is_prime_check = PrimalityTests.isProbablePrimeMillerRabin(
				p,
				miller_rabin_iterations
			);
			results.add(
				new ExperimentResult(i, p, (is_prime_check ? '+' : '-'), k[0])
			);
		}
		printResultsTable(results);
		results.clear();

		System.out.println("Генерация 10 простых чисел по тесту Миллера\n");
		for (int i = 0; i < 10; i++) {
			int[] k = { 0 };
			long p = PrimalityTests.generatePrimeMillerRabin(p_bits, k);
			boolean is_prime_check = PrimalityTests.isProbablePrimeMillerRabin(
				p,
				miller_rabin_iterations
			);
			results.add(
				new ExperimentResult(i, p, (is_prime_check ? '+' : '-'), k[0])
			);
		}
		printResultsTable(results);
		results.clear();

		System.out.println("Генерация 10 простых чисел по тесту Поклингтона\n");
		for (int i = 0; i < 10; i++) {
			int[] k = { 0 };
			long p = PrimalityTests.generatePrimePocklington(p_bits, k);
			boolean is_prime_check = PrimalityTests.isProbablePrimeMillerRabin(
				p,
				miller_rabin_iterations
			);
			results.add(
				new ExperimentResult(i, p, (is_prime_check ? '+' : '-'), k[0])
			);
		}
		printResultsTable(results);
		results.clear();
	}
}
