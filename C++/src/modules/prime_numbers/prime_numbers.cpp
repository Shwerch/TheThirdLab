#include <bits/stdc++.h>
using namespace std;
using u64 = uint64_t;
using i64 = int64_t;

// ------------------ утилиты ------------------
u64 mul_mod(u64 a, u64 b, u64 mod) {
    // безопасное умножение по модулю для 64-бит, используя 128-bit промежуточное
    __uint128_t z = ( __uint128_t ) a * b;
    return (u64)(z % mod);
}
u64 pow_mod(u64 a, u64 d, u64 mod) {
    u64 res = 1 % mod;
    a %= mod;
    while (d) {
        if (d & 1) res = mul_mod(res, a, mod);
        a = mul_mod(a, a, mod);
        d >>= 1;
    }
    return res;
}

// простой решето Эратосфена для простых < limit (включительно)
vector<uint32_t> sieve_primes_under(int limit) {
    vector<char> is(limit+1, true);
    is[0] = is[1] = false;
    for (int p = 2; p * p <= limit; ++p) if (is[p]) {
        for (int q = p*p; q <= limit; q += p) is[q] = false;
    }
    vector<uint32_t> out;
    for (int i = 2; i <= limit; ++i) if (is[i]) out.push_back((uint32_t)i);
    return out;
}

// Miller-Rabin (вероятностный). iterations — количество раундов.
// Для генератора случайных оснований используется mt19937_64.
bool miller_rabin(u64 n, int iterations, std::mt19937_64 &rng) {
    if (n < 2) return false;
    static const u64 smallPrimes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (u64 p : smallPrimes) {
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    // представим n-1 = d * 2^s
    u64 d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    uniform_int_distribution<u64> dist(2, n-2);
    for (int it = 0; it < iterations; ++it) {
        u64 a = dist(rng);
        u64 x = pow_mod(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true; // probably prime
}

// проверка делением по малым простым (< 500) — возвращаем делитель (0 если нет делителя)
u64 small_prime_divisor(u64 n, const vector<uint32_t> &small_primes) {
    for (uint32_t p : small_primes) {
        if ((u64)p == n) return 0; // равное малое простое => не считать делителем
        if ((u64)p * (u64)p > n) break; // дальше не надо
        if (n % p == 0) return p;
    }
    return 0;
}

// генерация случайного непарного числа строго в диапазоне [low, high], используя rng
u64 random_odd_in_range(u64 low, u64 high, std::mt19937_64 &rng) {
    if (low > high) swap(low, high);
    uniform_int_distribution<u64> dist(low, high);
    for (int tries=0; tries<1000000; ++tries) {
        u64 x = dist(rng);
        if ((x & 1) == 0) x ^= 1; // сделать непарным
        if (x >= low && x <= high) return x;
    }
    return low | 1ULL;
}

// ------------------ генераторы/процедуры ------------------

// 1) Miller-based generator: пробуем случайные числа нужной длины,
// фильтруем trial-division по малым простым и затем Miller-Rabin.
// Возвращаем вектор найденных простых (всего wantCount).
// В k_count записываем число случаев: trial-division нашло делитель,
// но probabilistic (MR) всё равно вернул "probably prime" (редкая аномалия).
vector<u64> generate_by_miller(int bitlen, int wantCount, const vector<uint32_t> &small_primes, int mr_rounds, std::mt19937_64 &rng, int &k_count) {
    vector<u64> found;
    k_count = 0;
    if (bitlen < 2 || bitlen > 64) return found;
    u64 low = (bitlen == 64) ? (1ULL<<63) : (1ULL << (bitlen-1));
    u64 high = (bitlen == 64) ? numeric_limits<u64>::max() : ((1ULL<<bitlen)-1);
    int tries = 0;
    while ((int)found.size() < wantCount && tries < 10000000) {
        ++tries;
        u64 cand = random_odd_in_range(low, high, rng);
        // trial division by small primes (<500) to quickly reject trivial composites
        u64 div = small_prime_divisor(cand, small_primes);
        // Now run MR
        bool mr = miller_rabin(cand, mr_rounds, rng);
        if (div != 0 && mr) {
            // небольшая аномалия — trial division нашло делитель, но MR сказал "probably prime"
            ++k_count;
        }
        if (mr) {
            // приём
            found.push_back(cand);
        }
    }
    return found;
}

// 2) Pocklington-based generator:
// Формируем фактор A как произведение малых простых (из small_primes) до тех пор,
// пока A > sqrt(2^bitlen) (т.е. > 2^{bitlen/2}). Затем ищем r в диапазоне
// [ceil(2^{bitlen-1}/A), floor((2^bitlen -1)/A)] и считаем n = A*r + 1.
// Применяем обобщ. Pocklington: для каждого простого p|A ищем a_p,
// удовлетворяющие условиям. Если нашли — n считается простым.
vector<u64> generate_by_pocklington(int bitlen, int wantCount, const vector<uint32_t> &small_primes, int mr_rounds, std::mt19937_64 &rng, int &k_count) {
    vector<u64> found;
    k_count = 0;
    if (bitlen < 3 || bitlen > 64) return found;
    // A: нарастим произведение малых простых пока A > 2^{bitlen/2}
    long double threshold_ld = pow((long double)2.0L, (long double)bitlen / 2.0L);
    u64 A = 1;
    vector<uint32_t> Afactors;
    for (uint32_t p : small_primes) {
        if (A > (u64)1e18) break; // защита от переполнения
        A *= p;
        Afactors.push_back(p);
        if ((long double)A > threshold_ld) break;
    }
    if (A <= 1) return found; // не накопили фактор
    // диапазон для r
    u64 rng_low = (bitlen==64) ? ( ( ( (__uint128_t)1 << 63 ) ) / A ) : ( (1ULL<<(bitlen-1)) / A );
    u64 rng_high = (bitlen==64) ? ( numeric_limits<u64>::max() / A ) : ( ((1ULL<<bitlen)-1) / A );
    if (rng_low < 1) rng_low = 1;
    int tries = 0;
    while ((int)found.size() < wantCount && tries < 2000000) {
        ++tries;
        u64 r = random_odd_in_range(rng_low, rng_high, rng);
        if (std::gcd(r, A) != 1) continue;
        // n = A * r + 1
        __uint128_t tmp = ( __uint128_t ) A * r + 1;
        if (tmp > numeric_limits<u64>::max()) continue;
        u64 n = (u64)tmp;
        // Теперь для каждого p | A нужно найти a_p:
        bool ok = true;
        for (uint32_t p : Afactors) {
            bool found_a = false;
            // пробуем несколько a (2..200)
            for (u64 a = 2; a <= 200; ++a) {
                if (a % n == 0) continue;
                u64 v = pow_mod(a, n-1, n);
                if (v != 1) { ok = false; break; } // условие (a^{n-1} == 1 mod n) нужно для всех a
                u64 t = (n-1) / p;
                u64 gval = pow_mod(a, t, n);
                // gcd(gval - 1, n) == 1
                u64 g = std::gcd((u64)((gval + n - 1) % n), n); // (gval - 1) mod n
                if (g == 1) { found_a = true; break; }
            }
            if (!ok) break;
            if (!found_a) { ok = false; break; }
        }
        if (ok) {
            // проверка малыми простыми и MR (как дополнительный шаг)
            u64 div = small_prime_divisor(n, small_primes);
            bool mr = miller_rabin(n, mr_rounds, rng);
            if (div != 0 && mr) ++k_count;
            if (mr) found.push_back(n);
        }
    }
    return found;
}

// 3) GOST R 34.10-94 (адаптированная / упрощённая версия, подходящая под 64-бит):
// Идея (упрощённо, в духе процедуры А): сначала генерируем q (половина битов),
// затем вычисляем N = floor(2^{bitlen-1} / q) и смотрим p = q*(N + k) + 1, k=0,2,4...
// проверяем Pocklington-подобные условия (как в ГОСТ) и MR.
// Для настоящих 512/1024-бит ГОСТ-процедуры нужны большие числа (bignum),
// здесь мы демонстрируем логику в 64-битных рамках.
vector<u64> generate_by_gostA(int bitlen, int wantCount, const vector<uint32_t> &small_primes, int mr_rounds, std::mt19937_64 &rng, int &k_count) {
    vector<u64> found;
    k_count = 0;
    if (bitlen < 4 || bitlen > 64) return found;
    int qbits = max(2, bitlen / 2); // генерируем q примерно половинного размера
    // Генерация q: простые qbits (простая MR-процедура с trial-division)
    int attempts = 0;
    while ((int)found.size() < wantCount && attempts < 2000000) {
        ++attempts;
        // генерируем q
        u64 qlow = (qbits == 64) ? (1ULL<<63) : (1ULL << (qbits-1));
        u64 qhigh = (qbits == 64) ? numeric_limits<u64>::max() : ((1ULL<<qbits) - 1);
        u64 q = random_odd_in_range(qlow, qhigh, rng);
        if (small_prime_divisor(q, small_primes) != 0) continue;
        if (!miller_rabin(q, mr_rounds, rng)) continue;
        // теперь строим p = q*(N + k) + 1, где N = floor(2^{bitlen-1} / q)
        __uint128_t two_pow = (__uint128_t)1 << (bitlen - 1);
        __uint128_t Nbig = two_pow / q;
        if (Nbig == 0) continue;
        // k перебираем 0,2,4,6,...
        for (u64 k = 0; k < (1ULL<<20); k += 2) {
            __uint128_t pbig = (__uint128_t)q * (Nbig + k) + 1;
            if (pbig > numeric_limits<u64>::max()) break;
            u64 p = (u64)pbig;
            if (p < (1ULL << (bitlen-1)) || p >= ((bitlen==64)?numeric_limits<u64>::max(): (1ULL<<bitlen))) continue;
            // проверка условий, аналогичных ГОСТ: a^{p-1} == 1 (mod p) и gcd(a^{(p-1)/q} - 1, p) == 1
            bool ok = false;
            for (u64 a = 2; a <= 200; ++a) {
                if (a % p == 0) continue;
                if (pow_mod(a, p-1, p) != 1) { ok = false; break; }
                u64 gval = pow_mod(a, (p-1)/q, p);
                u64 g = std::gcd((u64)((gval + p - 1) % p), p);
                if (g == 1) { ok = true; break; }
            }
            if (!ok) continue;
            // дополнительная проверка малыми простыми / MR
            u64 div = small_prime_divisor(p, small_primes);
            bool mr_ok = miller_rabin(p, mr_rounds, rng);
            if (div != 0 && mr_ok) ++k_count;
            if (mr_ok) {
                found.push_back(p);
                break;
            }
        }
    }
    return found;
}

// ------------------ вывод таблиц ------------------
void print_table_and_stats(const string &name, const vector<u64> &vec, int mr_rounds, std::mt19937_64 &rng, int k_count) {
    cout << "=== Метод: " << name << " ===\n";
    cout << "№\tP\t\tПроверка (MR x" << mr_rounds << ")\n";
    for (size_t i = 0; i < vec.size(); ++i) {
        u64 n = vec[i];
        bool mr = miller_rabin(n, mr_rounds, rng);
        cout << (i+1) << "\t" << n << "\t" << (mr ? "+" : "-") << "\n";
    }
    cout << "K = " << k_count << endl << endl;
}

// ------------------ main ------------------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // ----------------- параметры -----------------
    const int LIMIT_SMALL_PRIMES = 500;
    const int BITLEN = 31; // длина в битах (по умолчанию 31 — удобно для демонстрации). Можно поменять (<=64)
    const int WANT_COUNT = 10; // 10 простых чисел от каждого метода

    // Определяем число итераций Miller-Rabin так, чтобы вероятность ошибки <= 0.1.
    // Верхняя оценка ошибки MR за k итераций: <= 4^{-k}.
    // Хотим 4^{-k} <= 0.1 => k >= ceil( log(0.1) / log(0.25) ) = 2 (т.к. 0.25^2 = 0.0625 <= 0.1).
    const int MR_ROUNDS = 2;

    // ----------------- rng -----------------
    std::random_device rd;
    std::mt19937_64 rng((uint64_t) (chrono::steady_clock::now().time_since_epoch().count() ^ (uint64_t)rd()));
    // ----------------- Шаг 1: простые < 500 -----------------
    auto small_primes = sieve_primes_under(LIMIT_SMALL_PRIMES);
    cout << "Простые числа < " << LIMIT_SMALL_PRIMES << " (" << small_primes.size() << " штук):\n";
    for (size_t i = 0; i < small_primes.size(); ++i) {
        cout << small_primes[i];
        if ((i+1) % 10 == 0) cout << "\n"; else cout << "\t";
    }
    cout << "\n\n";

    // ----------------- Шаг 2: генерация разными методами -----------------
    int k_miller = 0;
    auto res_miller = generate_by_miller(BITLEN, WANT_COUNT, small_primes, MR_ROUNDS, rng, k_miller);
    print_table_and_stats("Miller-Rabin генерация", res_miller, MR_ROUNDS, rng, k_miller);

    int k_pock = 0;
    auto res_pock = generate_by_pocklington(BITLEN, WANT_COUNT, small_primes, MR_ROUNDS, rng, k_pock);
    print_table_and_stats("Pocklington генерация", res_pock, MR_ROUNDS, rng, k_pock);

    int k_gost = 0;
    auto res_gost = generate_by_gostA(BITLEN, WANT_COUNT, small_primes, MR_ROUNDS, rng, k_gost);
    print_table_and_stats("GOST R 34.10-94 генерация", res_gost, MR_ROUNDS, rng, k_gost);

    return 0;
}
