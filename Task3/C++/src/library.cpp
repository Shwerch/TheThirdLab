#include "library.h" // Подключаем заголовочный файл нашей библиотеки

long long pow(int base, int exp) {
	long long result = 1;
	for (int i = 0; i < exp; ++i)
		result *= base;
	return result;
}

// Вычисляет наибольший общий делитель двух чисел
long long gcd(long long numA, long long numB) { return std::gcd(numA, numB); }

// Сокращает рациональную дробь до несократимого вида
Rational simplifyRational(long long numer, long long denom) {
	if (denom == 0) {
		// Обработка деления на ноль, хотя в контексте задачи это не должно произойти
		return {numer, denom};
	}
	long long commonDivisor = gcd(std::abs(numer), std::abs(denom));
	long long simplifiedNumer = numer / commonDivisor;
	long long simplifiedDenom = denom / commonDivisor;

	// Гарантируем, что знаменатель всегда положителен
	if (simplifiedDenom < 0) {
		simplifiedNumer = -simplifiedNumer;
		simplifiedDenom = -simplifiedDenom;
	}
	return {simplifiedNumer, simplifiedDenom};
}

// Вычисляет и печатает сумму ряда
void calculateAndPrintSeriesSum(int valA, int valB) {
	// Проверка на расходимость ряда
	if (valB == 1) {
		std::cout << "Ряд расходится: infinity" << std::endl;
		return;
	}

	// Ряд является сходящимся, если |b| > 1.
	// Сумма ряда n^a / b^n может быть представлена в виде полинома от 1/(b-1)
	// умноженного на (b-1)^(a+1) / b^a.
	// Это сложный полином (например, для a=1 это b/(b-1)^2, для a=2 это b(b+1)/(b-1)^3 и т.д.)
	// Вместо прямой формулы, которая усложнит задачу с рациональными числами,
	// рассмотрим частные случаи или используем численное суммирование до большого N.
	// Однако, поскольку ответ требуется в виде несократимой дроби, численное суммирование не
	// подходит. Для данной задачи, где a и b малы, можно использовать формулы для сумм степеней.

	// Для a=1: Sum(n/b^n) = b / (b-1)^2
	// Для a=2: Sum(n^2/b^n) = b(b+1) / (b-1)^3
	// Для a=3: Sum(n^3/b^n) = b(b^2+4b+1) / (b-1)^4
	// Для a=4: Sum(n^4/b^n) = b(b^3+11b^2+11b+1) / (b-1)^5

	long long numerSum = 0; // Числитель суммы
	long long denomSum = 1; // Знаменатель суммы

	// Использование вектора для хранения коэффициентов полинома Бернулли или его производных
	// Для n^a / x^n, сумма является рациональной функцией от x.
	// Коэффициенты для числителя зависят от 'a' и имеют вид полинома от 'b'.
	// Знаменатель всегда (b-1)^(a+1)

	// Вектор для хранения коэффициентов полинома Бернулли для S_k(x) = Sum_{n=0 to inf} n^k x^n
	// Здесь мы имеем Sum_{n=1 to inf} n^a / b^n
	// Для удобства, будем использовать b в качестве x
	// S_0(x) = x/(1-x)
	// S_1(x) = x/(1-x)^2
	// S_2(x) = x(1+x)/(1-x)^3
	// S_3(x) = x(1+4x+x^2)/(1-x)^4
	// S_4(x) = x(1+11x+11x^2+x^3)/(1-x)^5

	// Меняем x на 1/b:
	// S_1(1/b) = (1/b) / (1-1/b)^2 = (1/b) / ((b-1)/b)^2 = (1/b) / ((b-1)^2 / b^2) = b / (b-1)^2
	// S_2(1/b) = (1/b)(1+1/b) / (1-1/b)^3 = (1/b)((b+1)/b) / ((b-1)/b)^3 = ((b+1)/b^2) / ((b-1)^3 /
	// b^3) = b(b+1) / (b-1)^3 S_3(1/b) = (1/b)(1+4/b+1/b^2) / (1-1/b)^4 = (1/b)((b^2+4b+1)/b^2) /
	// ((b-1)^4/b^4) = ((b^2+4b+1)/b^3) / ((b-1)^4/b^4) = b(b^2+4b+1) / (b-1)^4 S_4(1/b) =
	// (1/b)(1+11/b+11/b^2+1/b^3) / (1-1/b)^5 = (1/b)((b^3+11b^2+11b+1)/b^3) / ((b-1)^5/b^5) =
	// b(b^3+11b^2+11b+1) / (b-1)^5

	// Используем векторы для хранения коэффициентов числителя
	std::vector<long long> numCoeffs;

	// Вычисляем числитель и знаменатель для каждого 'a'
	// long long currentB = 1; // Для возведения b в степень
	switch (valA) {
	case 1:
		numerSum = valB;
		denomSum = pow(valB - 1, 2);
		break;
	case 2:
		numerSum = (long long)valB * (valB + 1);
		denomSum = pow(valB - 1, 3);
		break;
	case 3:
		// numCoeffs = {1, 4, 1}; // Коэффициенты для (b^2+4b+1)
		numerSum = (long long)valB * (pow(valB, 2) + 4 * valB + 1);
		denomSum = pow(valB - 1, 4);
		break;
	case 4:
		// numCoeffs = {1, 11, 11, 1}; // Коэффициенты для (b^3+11b^2+11b+1)
		numerSum = (long long)valB * (pow(valB, 3) + 11 * pow(valB, 2) + 11 * valB + 1);
		denomSum = pow(valB - 1, 5);
		break;
	case 5:
		// numCoeffs = {1, 26, 66, 26, 1};
		numerSum = (long long)valB *
				   (pow(valB, 4) + 26 * pow(valB, 3) + 66 * pow(valB, 2) + 26 * valB + 1);
		denomSum = pow(valB - 1, 6);
		break;
	case 6:
		// numCoeffs = {1, 57, 302, 302, 57, 1};
		numerSum = (long long)valB * (pow(valB, 5) + 57 * pow(valB, 4) + 302 * pow(valB, 3) +
									  302 * pow(valB, 2) + 57 * valB + 1);
		denomSum = pow(valB - 1, 7);
		break;
	case 7:
		// numCoeffs = {1, 120, 1191, 2416, 1191, 120, 1};
		numerSum = (long long)valB * (pow(valB, 6) + 120 * pow(valB, 5) + 1191 * pow(valB, 4) +
									  2416 * pow(valB, 3) + 1191 * pow(valB, 2) + 120 * valB + 1);
		denomSum = pow(valB - 1, 8);
		break;
	case 8:
		// numCoeffs = {1, 247, 4293, 15619, 15619, 4293, 247, 1};
		numerSum = (long long)valB *
				   (pow(valB, 7) + 247 * pow(valB, 6) + 4293 * pow(valB, 5) + 15619 * pow(valB, 4) +
					15619 * pow(valB, 3) + 4293 * pow(valB, 2) + 247 * valB + 1);
		denomSum = pow(valB - 1, 9);
		break;
	case 9:
		// numCoeffs = {1, 502, 14608, 88234, 156190, 88234, 14608, 502, 1};
		numerSum = (long long)valB * (pow(valB, 8) + 502 * pow(valB, 7) + 14608 * pow(valB, 6) +
									  88234 * pow(valB, 5) + 156190 * pow(valB, 4) +
									  88234 * pow(valB, 3) + 14608 * pow(valB, 2) + 502 * valB + 1);
		denomSum = pow(valB - 1, 10);
		break;
	case 10:
		// numCoeffs = {1, 1013, 47840, 455192, 1310356, 1310356, 455192, 47840, 1013, 1};
		numerSum = (long long)valB *
				   (pow(valB, 9) + 1013 * pow(valB, 8) + 47840 * pow(valB, 7) +
					455192 * pow(valB, 6) + 1310356 * pow(valB, 5) + 1310356 * pow(valB, 4) +
					455192 * pow(valB, 3) + 47840 * pow(valB, 2) + 1013 * valB + 1);
		denomSum = pow(valB - 1, 11);
		break;
	default:
		// Этот случай не должен быть достигнут из-за проверки ввода
		std::cout << "Неизвестное значение 'a'." << std::endl;
		return;
	}

	Rational resultFrac = simplifyRational(numerSum, denomSum);

	// Поскольку 'a' и 'b' являются целыми числами, а формулы для суммирования
	// рядов с n^a/b^n дают рациональные числа, результат всегда будет рациональным.
	// Иррациональность в этом случае невозможна.
	std::cout << "Сумма ряда: " << resultFrac.numer << "/" << resultFrac.denom << " ("
			  << resultFrac.numer / static_cast<double>(resultFrac.denom) << ")" << std::endl;
}
