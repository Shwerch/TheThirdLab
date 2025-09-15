import java.util.InputMismatchException;
import java.util.Scanner;
import java.util.Vector;

class Rational {

	public long numer; // Числитель
	public long denom; // Знаменатель

	public Rational(long numer, long denom) {
		this.numer = numer;
		this.denom = denom;
	}
}

public class MathSum {

	// Custom power function for long to avoid Math.pow(double, double) precision
	// issues
	private static long pow(long base, int exp) {
		long result = 1;
		for (int i = 0; i < exp; ++i) {
			result *= base;
		}
		return result;
	}

	// Вычисляет наибольший общий делитель двух чисел
	private static long gcd(long numA, long numB) {
		return java.math.BigInteger.valueOf(numA)
			.gcd(java.math.BigInteger.valueOf(numB))
			.longValue();
	}

	// Сокращает рациональную дробь до несократимого вида
	private static Rational simplifyRational(long numer, long denom) {
		if (denom == 0) {
			// Обработка деления на ноль, хотя в контексте задачи это не должно произойти
			return new Rational(numer, denom);
		}
		long commonDivisor = gcd(Math.abs(numer), Math.abs(denom));
		long simplifiedNumer = numer / commonDivisor;
		long simplifiedDenom = denom / commonDivisor;

		// Гарантируем, что знаменатель всегда положителен
		if (simplifiedDenom < 0) {
			simplifiedNumer = -simplifiedNumer;
			simplifiedDenom = -simplifiedDenom;
		}
		return new Rational(simplifiedNumer, simplifiedDenom);
	}

	// Вычисляет и печатает сумму ряда
	private static void calculateAndPrintSeriesSum(int valA, int valB) {
		// Проверка на расходимость ряда
		if (valB == 1) {
			System.out.println("Ряд расходится: infinity");
			return;
		}

		long numerSum = 0; // Числитель суммы
		long denomSum = 1; // Знаменатель суммы

		switch (valA) {
			case 1:
				numerSum = valB;
				denomSum = pow(valB - 1, 2);
				break;
			case 2:
				numerSum = (long) valB * (valB + 1);
				denomSum = pow(valB - 1, 3);
				break;
			case 3:
				numerSum = (long) valB * (pow(valB, 2) + 4 * valB + 1);
				denomSum = pow(valB - 1, 4);
				break;
			case 4:
				numerSum =
					(long) valB *
					(pow(valB, 3) + 11 * pow(valB, 2) + 11 * valB + 1);
				denomSum = pow(valB - 1, 5);
				break;
			case 5:
				numerSum =
					(long) valB *
					(pow(valB, 4) +
						26 * pow(valB, 3) +
						66 * pow(valB, 2) +
						26 * valB +
						1);
				denomSum = pow(valB - 1, 6);
				break;
			case 6:
				numerSum =
					(long) valB *
					(pow(valB, 5) +
						57 * pow(valB, 4) +
						302 * pow(valB, 3) +
						302 * pow(valB, 2) +
						57 * valB +
						1);
				denomSum = pow(valB - 1, 7);
				break;
			case 7:
				numerSum =
					(long) valB *
					(pow(valB, 6) +
						120 * pow(valB, 5) +
						1191 * pow(valB, 4) +
						2416 * pow(valB, 3) +
						1191 * pow(valB, 2) +
						120 * valB +
						1);
				denomSum = pow(valB - 1, 8);
				break;
			case 8:
				numerSum =
					(long) valB *
					(pow(valB, 7) +
						247 * pow(valB, 6) +
						4293 * pow(valB, 5) +
						15619 * pow(valB, 4) +
						15619 * pow(valB, 3) +
						4293 * pow(valB, 2) +
						247 * valB +
						1);
				denomSum = pow(valB - 1, 9);
				break;
			case 9:
				numerSum =
					(long) valB *
					(pow(valB, 8) +
						502 * pow(valB, 7) +
						14608 * pow(valB, 6) +
						88234 * pow(valB, 5) +
						156190 * pow(valB, 4) +
						88234 * pow(valB, 3) +
						14608 * pow(valB, 2) +
						502 * valB +
						1);
				denomSum = pow(valB - 1, 10);
				break;
			case 10:
				numerSum =
					(long) valB *
					(pow(valB, 9) +
						1013 * pow(valB, 8) +
						47840 * pow(valB, 7) +
						455192 * pow(valB, 6) +
						1310356 * pow(valB, 5) +
						1310356 * pow(valB, 4) +
						455192 * pow(valB, 3) +
						47840 * pow(valB, 2) +
						1013 * valB +
						1);
				denomSum = pow(valB - 1, 11);
				break;
			default:
				System.out.println("Неизвестное значение 'a'.");
				return;
		}

		Rational resultFrac = simplifyRational(numerSum, denomSum);

		System.out.println(
			"Сумма ряда: " +
			resultFrac.numer +
			"/" +
			resultFrac.denom +
			" (" +
			(double) resultFrac.numer / resultFrac.denom +
			")"
		);
	}

	public static void calculateMathSum() {
		Scanner scanner = new Scanner(System.in);

		System.out.println(new String(new char[30]).replace('\0', '-'));
		System.out.println("Программа для вычисления суммы ряда");
		System.out.println(new String(new char[30]).replace('\0', '-'));

		int valA = 0;
		int valB = 0;

		while (true) {
			System.out.print("Введите целое число 'a' (от 1 до 10): ");
			try {
				valA = scanner.nextInt();
				if (valA >= 1 && valA <= 10) {
					break;
				} else {
					System.out.println(
						"Некорректный ввод. Пожалуйста, введите число от 1 до 10."
					);
				}
			} catch (InputMismatchException e) {
				System.out.println(
					"Некорректный ввод. Пожалуйста, введите число от 1 до 10."
				);
				scanner.next(); // consume the invalid input
			}
		}

		while (true) {
			System.out.print("Введите целое число 'b' (от 1 до 10): ");
			try {
				valB = scanner.nextInt();
				if (valB >= 1 && valB <= 10) {
					break;
				} else {
					System.out.println(
						"Некорректный ввод. Пожалуйста, введите число от 1 до 10."
					);
				}
			} catch (InputMismatchException e) {
				System.out.println(
					"Некорректный ввод. Пожалуйста, введите число от 1 до 10."
				);
				scanner.next(); // consume the invalid input
			}
		}

		calculateAndPrintSeriesSum(valA, valB);
	}
}
