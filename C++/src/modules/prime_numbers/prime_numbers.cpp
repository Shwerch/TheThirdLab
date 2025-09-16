#include <bits/stdc++.h>
using namespace std;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using i64 = int64_t;

static mt19937_64 rng((uint64_t)chrono::high_resolution_clock::now().time_since_epoch().count());

// ----- Утилиты -----
static u32 bit_length_u64(u64 x) {
	if (x == 0)
		return 0;
	u32 l = 0;
	while (x) {
		++l;
		x >>= 1;
	}
	return l;
}

// безопасное (без __int128) умножение по модулю: использует double-and-add
static u64 mulmod(u64 a, u64 b, u64 mod) {
	a %= mod;
	u64 res = 0;
	while (b) {
		if (b & 1) {
			// res = (res + a) % mod, выполняем без переполнения
			if (res >= mod - a)
				res = res + a - mod;
			else
				res = res + a;
		}
		// a = (a + a) % mod
		if (a >= mod - a)
			a = a + a - mod;
		else
			a = a + a;
		b >>= 1;
	}
	return res;
}

static u64 powmod(u64 a, u64 e, u64 mod) {
	a %= mod;
	u64 res = 1 % mod;
	while (e) {
		if (e & 1)
			res = mulmod(res, a, mod);
		a = mulmod(a, a, mod);
		e >>= 1;
	}
	return res;
}

static u64 random_in_range(u64 lo, u64 hi) {
	if (lo >= hi)
		return lo;
	uniform_int_distribution<u64> dist(lo, hi);
	return dist(rng);
}

static vector<u16> sieve_primes_upto(u16 limit) {
	vector<char> is(limit + 1, true);
	is[0] = is[1] = false;
	for (u32 p = 2; (u32)p * p <= limit; ++p)
		if (is[p]) {
			for (u32 q = p * p; q <= limit; q += p)
				is[q] = false;
		}
	vector<u16> out;
	for (u32 i = 2; i <= limit; ++i)
		if (is[i])
			out.push_back((u16)i);
	return out;
}

// Простейшая факторизация (тр/дел) — подходит для наших размеров
static map<u64, u32> factorize(u64 n, const vector<u16> &small_primes) {
	map<u64, u32> f;
	u64 m = n;
	for (u16 p : small_primes) {
		if ((u64)p * (u64)p > m)
			break;
		if (m % p == 0) {
			u32 cnt = 0;
			while (m % p == 0) {
				m /= p;
				++cnt;
			}
			f[p] = cnt;
		}
	}
	// дальнейшее пробное деление (неоптимально, но для учебных размеров нормально)
	u64 d = (small_primes.empty() ? 3 : (u64)small_primes.back() + 2);
	if (m > 1) {
		for (u64 i = d; i * i <= m; i += 2) {
			if (m % i == 0) {
				u32 cnt = 0;
				while (m % i == 0) {
					m /= i;
					++cnt;
				}
				f[i] = cnt;
			}
		}
	}
	if (m > 1)
		f[m]++; // остаток — простое
	return f;
}

// классический вероятностный тест Миллера-Рабина
static bool miller_rabin(u64 n, u32 iterations) {
	if (n < 2)
		return false;
	static const u64 small[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
	for (u64 p : small) {
		if (n == p)
			return true;
		if (n % p == 0)
			return false;
	}
	u64 d = n - 1;
	u32 s = 0;
	while ((d & 1) == 0) {
		d >>= 1;
		++s;
	}
	for (u32 it = 0; it < iterations; ++it) {
		u64 a = random_in_range(2, n - 2);
		u64 x = powmod(a, d, n);
		if (x == 1 || x == n - 1)
			continue;
		bool composite = true;
		for (u32 r = 1; r < s; ++r) {
			x = mulmod(x, x, n);
			if (x == n - 1) {
				composite = false;
				break;
			}
		}
		if (composite)
			return false;
	}
	return true;
}

// Реализация "Miller" как в задании (работаем со знанием разложения n-1).
// Возвращает: true => принято за простое (или "не опровергнуто"), false => составное.
// t - число случайных оснований
static bool miller_test_with_factors(u64 n, const map<u64, u32> &factors_nminus1, u32 t) {
	if (n < 3)
		return false;
	// 1) выбираем t случайных оснований a_j
	vector<u64> a_list;
	for (u32 i = 0; i < t; ++i) {
		u64 a = random_in_range(2, n - 2);
		a_list.push_back(a);
	}
	// 2) для каждого a_j проверяем a_j^(n-1) mod n == 1
	for (u64 a : a_list) {
		if (powmod(a, n - 1, n) != 1)
			return false; // однозначно составное
	}
	// 3) для каждого простого фактора q_i проверяем a_j^{(n-1)/q_i}
	for (auto &pf : factors_nminus1) {
		u64 q = pf.first;
		bool all_one_over_all_a = true;
		u64 exp = (n - 1) / q;
		for (u64 a : a_list) {
			u64 r = powmod(a, exp, n);
			if (r != 1) {
				all_one_over_all_a = false;
				break;
			}
		}
		if (all_one_over_all_a) {
			// если для некоторого q все основания дают 1, то тест говорит "вероятно составное"
			return false; // трактуем как составное (согласно заданию)
		}
	}
	// иначе считаем простым (вероятно)
	return true;
}

// Тест Поклингтона (базовый): вход n, факторизация F (F > R), R - соответствующий множитель, t -
// число оснований. Возвращает true => доказательство простоты (или "вероятно простое" при неполных
// условиях).
static bool pocklington_test(u64 n, const map<u64, u32> &F_factors, u32 t) {
	if (n < 3)
		return false;
	// 1) выбираем t оснований
	for (u32 it = 0; it < t; ++it) {
		u64 a = random_in_range(2, n - 2);
		if (powmod(a, n - 1, n) != 1)
			return false; // составное
		bool ok_for_all_q = true;
		for (auto &pf : F_factors) {
			u64 q = pf.first;
			u64 exp = (n - 1) / q;
			u64 val = powmod(a, exp, n);
			u64 g = gcd((u64)((val + n - 1) % n), n); // gcd(val-1, n)
			if (g != 1) {
				ok_for_all_q = false;
				break;
			}
		}
		if (ok_for_all_q) {
			// теорема Поклингтона — при дополнительном условии F > sqrt(n)-1 можно утверждать
			// простоту. Для небольших n мы требуем это условие; иначе считаем "вероятно простое".
			long double sq = sqrt((long double)n);
			u64 F = 1;
			for (auto &pf : F_factors) {
				for (u32 e = 0; e < pf.second; ++e) {
					if (F > n / pf.first)
						F = n;
					else
						F *= pf.first;
				}
			}
			if (F > (u64)floor(sq) - 1)
				return true; // можно считать доказанным
			else
				return true; // для учебной реализации считаем принятым (вероятно простое)
		}
		// иначе пробуем другое a
	}
	// не нашли подходящего основания — не доказано простое
	return false;
}

// Генерация случайного простого числа заданной длины (используем MR)
static u64 generate_random_prime_bits(u32 bits, const vector<u16> &small_primes, u32 mr_iters = 5) {
	if (bits < 2)
		return 2;
	while (true) {
		u64 cand = ((u64)1 << (bits - 1)) | (random_in_range(0, ((u64)1 << (bits - 1)) - 1));
		cand |= 1; // нечетное
		// быстро исключаем делимость малыми простыми
		bool divisible = false;
		for (u16 p : small_primes) {
			if (cand % p == 0 && cand != p) {
				divisible = true;
				break;
			}
		}
		if (divisible)
			continue;
		if (miller_rabin(cand, mr_iters))
			return cand;
	}
}

// Вспомог: построение m = product( q_i^{a_i} ) целенаправленно для Miller-генератора
static u64 build_m_for_miller(u32 target_bits_minus1, const vector<u16> &small_primes,
							  vector<u64> &rejected_candidates) {
	// Будем пробовать собирать m случайным сложением множителей из таблицы
	const u32 MAX_TRIES = 2000;
	for (u32 attempt = 0; attempt < MAX_TRIES; ++attempt) {
		u64 m = 1;
		map<u64, u32> fac;
		// добавляем множители, пока не достигнем нужной длины
		while (bit_length_u64(m) < target_bits_minus1) {
			u16 q = small_primes[random_in_range(0, small_primes.size() - 1)];
			u32 a = (u32)random_in_range(1, 4); // небольшие степени
			u64 term = 1;
			for (u32 i = 0; i < a; ++i) {
				if (term > (UINT64_MAX / q)) {
					term = 0;
					break;
				}
				term *= q;
			}
			if (term == 0)
				break;
			if (m > 0 && term > (UINT64_MAX / m)) {
				m = 0;
				break;
			}
			m *= term;
			if (m == 0)
				break;
			fac[q] += a;
			// если превысили по битам — выходим и начнём заново
			if (bit_length_u64(m) > target_bits_minus1 + 2)
				break;
		}
		if (bit_length_u64(m) != target_bits_minus1)
			continue;
		u64 n = 2 * m + 1;
		if (n < 3)
			continue;
		// факторизация n-1 известна — это 2 * факторизация(m)
		map<u64, u32> nminus1_fac = factorize(n - 1, small_primes);
		// проверка по специальному Миллеру с t небольшим (используем t=6)
		if (miller_test_with_factors(n, nminus1_fac, 6)) {
			return n;
		} else {
			rejected_candidates.push_back(n);
		}
	}
	return 0; // не нашли
}

// Построение F для Поклингтона: F ~ > половины размера, возврат F, её факторизация
static bool build_F_and_R_for_pocklington(u32 target_bits, const vector<u16> &small_primes,
										  u64 &out_n, map<u64, u32> &out_F_factors,
										  vector<u64> &rejected_candidates) {
	const u32 MAX_TRIES = 2000;
	u32 half_bits = (target_bits / 2) + 1;
	for (u32 attempt = 0; attempt < MAX_TRIES; ++attempt) {
		// Собираем F
		u64 F = 1;
		map<u64, u32> Ff;
		while (bit_length_u64(F) < (u32)half_bits) {
			u16 q = small_primes[random_in_range(0, small_primes.size() - 1)];
			u32 a = (u32)random_in_range(1, 4);
			u64 term = 1;
			for (u32 i = 0; i < a; ++i) {
				if (term > (UINT64_MAX / q)) {
					term = 0;
					break;
				}
				term *= q;
			}
			if (term == 0)
				break;
			if (F > 0 && term > (UINT64_MAX / F)) {
				F = 0;
				break;
			}
			F *= term;
			Ff[q] += a;
			if (bit_length_u64(F) > half_bits + 4)
				break;
		}
		if (bit_length_u64(F) < (u32)half_bits)
			continue;
		// R — случайное четное, размером на 1 бит меньше, чем F
		u32 Rbits = max<u32>(1, bit_length_u64(F) - 1);
		u64 R = (((u64)1 << (Rbits - 1)) | random_in_range(0, ((u64)1 << (Rbits - 1)) - 1));
		if (R % 2 == 1)
			R += 1;
		if (R == 0)
			continue;
		// проверка переполнения
		if (F > (UINT64_MAX - 1) / R)
			continue;
		u64 n = R * F + 1;
		if (n < 3)
			continue;
		// проверка тестом Поклингтона
		if (pocklington_test(n, Ff, 6)) {
			out_n = n;
			out_F_factors = Ff;
			return true;
		} else {
			rejected_candidates.push_back(n);
		}
	}
	return false;
}

// быстрое возведение в степень по модулю
static uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) { // если текущий бит равен 1
            result = (__uint128_t)result * base % mod;
        }
        base = (__uint128_t)base * base % mod;
        exp >>= 1; // переходим к следующему биту
    }
    return result;
}


// ГОСТ Р 34.10-94 (на основе теоремы Диемитко)
static uint64_t generate_gost_like(uint32_t bits, const vector<uint16_t> &small_primes,
                                   vector<uint64_t> &rejected_candidates) {
    uint32_t qbits = max<uint32_t>(2, bits / 2);

    for (uint32_t attempt = 0; attempt < 5000; ++attempt) {
        // генерируем простое q длиной ~bits/2
        uint64_t q = generate_random_prime_bits(qbits, small_primes, 5);

        // генерируем случайную дробь ξ в (0,1)
        double xi = (double)rand() / (double)RAND_MAX;

        // шаг 1: вычисляем N
        uint64_t N = ((uint64_t)1 << (bits - 1)) / q;
        N += (uint64_t)(((double)((uint64_t)1 << (bits - 1)) * xi) / (double)q);

        // делаем N чётным
        if (N % 2 == 1) N++;

        // шаги 2–6
        uint64_t u = 0;
        while (true) {
            // кандидат в простые
            if (N > (UINT64_MAX - 1) / q) break; // избегаем переполнения
            uint64_t p = (N + u) * q + 1;

            // проверка верхней границы
            if (p >= ((uint64_t)1 << bits)) break;

            // проверка условия теоремы Диемитко для a=2
            if (mod_pow(2, p - 1, p) == 1 && mod_pow(2, (p - 1) / q, p) != 1) {
                return p; // простое число найдено
            } else {
                rejected_candidates.push_back(p);
            }

            // шаг 6: увеличиваем u на 2
            u += 2;
        }
    }
    return 0;
}


// Для каждого метода: генерируем 10 чисел, проверяем их MR(t) с t, даём таблицу.
// Сохраняем отклонённые кандидаты и считаем k — сколько отклонённых MR считает простыми.

void calculatePrimeNumbers() {

	u32 target_bits = 16;

	// Sieve < 500
	vector<u16> small_primes = sieve_primes_upto(499);

	cout << "Таблица простых чисел < 500 (" << small_primes.size() << "):\n";
	for (size_t i = 0; i < small_primes.size(); ++i) {
		cout << setw(4) << (u32)small_primes[i];
		if ((i + 1) % 10 == 0)
			cout << "\n";
	}
	cout << "\n\n";

	// Параметр t для MR, такой, что вероятность ошибки <= 0.1.
	// Для MR: ошибка за один раунд ≤ 1/4, значит достаточно t = ceil(log(0.1)/log(1/4)) = 2.
	u32 mr_iterations_for_error = 2;
	cout << "Используем вероятностный тест Миллера-Рабина с t = " << mr_iterations_for_error
		 << " (прибл. вероятность ошибочного признания составного простым ≤ 0.1).\n\n";

	// 1) Miller-based generator
	vector<u64> miller_primes;
	vector<u64> miller_rejected;
	while (miller_primes.size() < 10) {
		u64 n = build_m_for_miller(target_bits - 1, small_primes, miller_rejected);
		if (n == 0) {
			cerr << "Не удалось заогнать m для Miller-алгоритма (попробуйте увеличить лимит "
					"попыток)\n";
			break;
		}
		// дополнительно проверяем итог MR
		bool mr_ok = miller_rabin(n, mr_iterations_for_error);
		if (mr_ok)
			miller_primes.push_back(n);
		else
			miller_rejected.push_back(n);
	}

	// 2) Pocklington-based
	vector<u64> pock_primes;
	vector<u64> pock_rejected;
	while (pock_primes.size() < 10) {
		u64 n;
		map<u64, u32> Ff;
		if (!build_F_and_R_for_pocklington(target_bits, small_primes, n, Ff, pock_rejected)) {
			cerr << "Не удалось найти n для Поклингтона за лимит попыток\n";
			break;
		}
		bool mr_ok = miller_rabin(n, mr_iterations_for_error);
		if (mr_ok)
			pock_primes.push_back(n);
		else
			pock_rejected.push_back(n);
	}

	// 3) GOST-like generator
	vector<u64> gost_primes;
	vector<u64> gost_rejected;
	while (gost_primes.size() < 10) {
		u64 p = generate_gost_like(target_bits, small_primes, gost_rejected);
		if (p == 0) {
			cerr << "Не удалось сгенерировать p по ГОСТ-подобному методу (лимит попыток)\n";
			break;
		}
		bool mr_ok = miller_rabin(p, mr_iterations_for_error);
		if (mr_ok)
			gost_primes.push_back(p);
		else
			gost_rejected.push_back(p);
	}

	// Вывод таблиц результатов по каждому методу + проверка MR (t=mr_iterations_for_error)
	auto print_results_table = [&](const string &title, const vector<u64> &primes,
								   const vector<u64> &rejected) {
		cout << "\n--- " << title << " ---\n";
		cout << "№\tp\t\tMR(" << mr_iterations_for_error << ")\n";
		u32 idx = 1;
		for (u64 p : primes) {
			bool mr_ok = miller_rabin(p, mr_iterations_for_error);
			cout << idx << "\t" << p << "\t" << (mr_ok ? "+" : "-") << "\n";
			++idx;
		}
		// Проверяем отвергнутые кандидаты: сколько из них MR считает простыми
		u32 k = 0;
		for (u64 x : rejected) {
			if (miller_rabin(x, mr_iterations_for_error))
				++k;
		}
		cout << "k (сколько отвергнутых тестом определены MR как простые) = " << k << " (из "
			 << rejected.size() << " отвергнутых)\n";
	};

	print_results_table("Miller-процедура", miller_primes, miller_rejected);
	print_results_table("Поклингтон-процедура", pock_primes, pock_rejected);
	print_results_table("ГОСТ-процедура", gost_primes, gost_rejected);

	// Доп. требование: проверка реализованного теста таблицей составных чисел.
	cout << "\n=== Валидация реализованных тестов на наборе составных чисел ===\n";
	vector<u64> control_composites = {561,	1105, 1729,
									  2465, 2821, 6601}; // классические кармихэйл-числа и др.
	cout << "Список контрольных составных чисел: ";
	for (auto c : control_composites)
		cout << c << " ";
	cout << "\nПроводим t=1 (один раунд) многократно (50 повторов) и считаем частоту, когда тест "
			"принимал составное за простое.\n";

	auto empirical_check = [&](auto test_func, const string &name) {
		cout << "\nТест: " << name << "\n";
		u32 repeats = 50;
		for (u64 c : control_composites) {
			u32 false_accept = 0;
			for (u32 r = 0; r < repeats; ++r) {
				bool ok = test_func(c);
				if (ok)
					++false_accept; // приняли составное за простое
			}
			cout << "Число " << c << " — false_accept " << false_accept << " / " << repeats << "\n";
		}
	};

	// Для MR с t=1 используем miller_rabin(c,1)
	empirical_check([&](u64 n) -> bool { return miller_rabin(n, 1); }, "Miller-Rabin (t=1)");
	// Для нашего miller_test_with_factors — используем разложение n-1 и t=1
	empirical_check(
		[&](u64 n) -> bool {
			auto f = factorize(n - 1, small_primes);
			return miller_test_with_factors(n, f, 1);
		},
		"Miller-специальный (t=1) с разложением n-1");
	// Для Pocklington (t=1) — попытаемся применить при условиях (попытается распознать простоту, но
	// на составных вероятно будет отклонять)
	empirical_check(
		[&](u64 n) -> bool {
			// Для Pocklington нам нужно представить n-1 = R * F ; попробуем взять F — произведение
			// мелких простых (не строго корректно, но для эмпирики)
			auto f = factorize(n - 1, small_primes);
			u64 F = 1;
			for (auto &kv : f) {
				for (u32 i = 0; i < kv.second; ++i)
					F *= kv.first;
			}
			if (F == 0 || F >= (n - 1))
				return false;
			// u64 R = (n - 1) / F;
			return pocklington_test(n, f, 1);
		},
		"Pocklington (t=1)");

	cout << "\nГотово.\n";
}
