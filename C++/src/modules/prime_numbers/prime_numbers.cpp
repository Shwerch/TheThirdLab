// Краткие поясняющие комментарии в коде.
// Везде для целых используются fixed-width типы: uint8_t..uint64_t и int8_t..int64_t.

#include <bits/stdc++.h>
#include <cstdint>
#include <random>
#include <iomanip>

using std::vector;
using std::string;
using std::cout;

// Простая реализация решета Эратосфена для чисел < limit
vector<uint16_t> sieve_primes_below(uint32_t limit) {
    vector<uint8_t> is_prime(limit, 1);
    is_prime[0] = is_prime[1] = 0;
    for (uint32_t p = 2; p * p < limit; ++p) {
        if (is_prime[p]) {
            for (uint32_t m = p * p; m < limit; m += p) is_prime[m] = 0;
        }
    }
    vector<uint16_t> primes;
    for (uint32_t i = 2; i < limit; ++i) if (is_prime[i]) primes.push_back(static_cast<uint16_t>(i));
    return primes;
}

uint64_t modmul(uint64_t a, uint64_t b, uint64_t mod) {
    a %= mod;
    uint64_t res = 0;
    while (b) {
        if (b & 1) {
            if (res >= mod - a) res = res - (mod - a);
            else res = res + a;
        }
        if (a >= mod - a) a = a - (mod - a);
        else a = a + a;
        b >>= 1;
    }
    return res;
}

// Быстрое возведение в степень по модулю
uint64_t modpow(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1) result = modmul(result, base, mod);
        base = modmul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

uint64_t ugcd(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t t = a % b;
        a = b; b = t;
    }
    return a;
}

// Сильный тест Миллера — вероятностный
bool miller_rabin_probable(uint64_t n, uint32_t rounds, std::mt19937_64 &rng, const vector<uint16_t> &small_primes) {
    if (n < 2) return false;
    for (uint16_t p : small_primes) {
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    uint64_t d = n - 1;
    uint32_t s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    std::uniform_int_distribution<uint64_t> dist(2, n - 2);
    for (uint32_t i = 0; i < rounds; ++i) {
        uint64_t a = dist(rng);
        uint64_t x = modpow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint32_t r = 1; r < s; ++r) {
            x = modmul(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

// Целочисленный квадратный корень (приближённо через double, затем корректировка)
uint64_t isqrt_u64(uint64_t x) {
    if (x == 0) return 0;
    long double d = std::sqrt((long double)x);
    uint64_t r = (uint64_t)d;
    while ((r+1) * (r+1) <= x) ++r;
    while (r * r > x) --r;
    return r;
}

// Тест Поклингтона: пытаемся получить детерминистическое доказательство простоты,
// факторизуя часть n-1 малыми простыми и проверяя свидетелей.
bool pocklington_prove_prime(uint64_t n, const vector<uint16_t> &small_primes, std::mt19937_64 &rng) {
    if (n < 2) return false;
    for (uint16_t p : small_primes) {
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    uint64_t n1 = n - 1;
    uint64_t A = 1; // произведение известных факторных частей
    vector<uint64_t> factors;
    for (uint16_t p : small_primes) {
        if (p > n1) break;
        if (n1 % p == 0) {
            uint64_t cur = 1;
            while (n1 % p == 0) { n1 /= p; cur *= p; }
            A *= cur;
            factors.push_back(p);
        }
        // если накопленное A стало больше sqrt(n) — достаточно
        uint64_t sqrt_n = isqrt_u64(n);
        if (A > sqrt_n) break;
    }
    if (A <= isqrt_u64(n)) return false; // не факторизовали достаточную часть n-1
    // Для каждого простого q | A нужно найти a: a^(n-1) ≡ 1 (mod n) и gcd(a^((n-1)/q)-1, n) = 1
    std::uniform_int_distribution<uint64_t> dist(2, n - 2);
    for (uint64_t q : factors) {
        bool ok_q = false;
        for (uint32_t attempt = 0; attempt < 8; ++attempt) {
            uint64_t a = dist(rng);
            if (modpow(a, n - 1, n) != 1) continue;
            uint64_t t = modpow(a, (n - 1) / q, n);
            uint64_t g = ugcd((t + n - 1) % n, n); // gcd(t-1, n)
            if (g == 1) { ok_q = true; break; }
        }
        if (!ok_q) return false;
    }
    return true;
}

// Утилиты генератора случайных чисел фиксированной битовой длины
uint64_t random_bits_uint(uint32_t bits, std::mt19937_64 &rng) {
    if (bits == 0) return 0;
    if (bits >= 64) bits = 63; // ограничение для безопасности
    uint64_t top = uint64_t(1) << (bits - 1);
    uint64_t mask = (bits == 64) ? ~uint64_t(0) : ((uint64_t(1) << bits) - 1);
    uint64_t x = rng() & mask;
    x |= top; // старший бит гарантирован
    x |= 1;   // делаем нечётным
    return x;
}

// Генератор по Миллеру: пробуем случайные кандидаты и принимаем те, что проходят MR
// Возвращаем: найденные числа, а также количество кандидатов, которые были отвергнуты, но MR показал "вероятно простые"
struct GenResult {
    vector<uint64_t> found;
    uint32_t false_rejected_by_method_but_probably_prime = 0;
};

GenResult generate_by_miller(uint32_t bits, uint32_t need, std::mt19937_64 &rng, const vector<uint16_t> &small_primes, uint32_t rounds_gen) {
    GenResult res;
    uint32_t attempts = 0;
    while (res.found.size() < need && attempts < 200000) {
        ++attempts;
        uint64_t cand = random_bits_uint(bits, rng);
        // быстрое отсеивание по маленьким простым
        bool divisible = false;
        for (uint16_t p : small_primes) {
            if (cand == p) { divisible = false; break; }
            if (cand % p == 0) { divisible = true; break; }
        }
        if (divisible) {
            // rejected by method (simple divisibility check); check MR probabilistic
            if (miller_rabin_probable(cand, 2, rng, small_primes)) ++res.false_rejected_by_method_but_probably_prime;
            continue;
        }
        if (miller_rabin_probable(cand, rounds_gen, rng, small_primes)) {
            res.found.push_back(cand);
        } else {
            if (miller_rabin_probable(cand, 2, rng, small_primes)) ++res.false_rejected_by_method_but_probably_prime;
        }
    }
    return res;
}

// Генератор по Поклингтону: пробуем кандидатов и пытаемся доказать простоту через факторизацию части n-1
GenResult generate_by_pocklington(uint32_t bits, uint32_t need, std::mt19937_64 &rng, const vector<uint16_t> &small_primes) {
    GenResult res;
    uint32_t attempts = 0;
    while (res.found.size() < need && attempts < 200000) {
        ++attempts;
        uint64_t cand = random_bits_uint(bits, rng);
        // малоформальный быстрый отбор
        bool divisible = false;
        for (uint16_t p : small_primes) {
            if (cand == p) { divisible = false; break; }
            if (cand % p == 0) { divisible = true; break; }
        }
        if (divisible) {
            if (miller_rabin_probable(cand, 2, rng, small_primes)) ++res.false_rejected_by_method_but_probably_prime;
            continue;
        }
        // пытаемся доказать простоту
        if (pocklington_prove_prime(cand, small_primes, rng)) {
            res.found.push_back(cand);
        } else {
            if (miller_rabin_probable(cand, 2, rng, small_primes)) ++res.false_rejected_by_method_but_probably_prime;
        }
    }
    return res;
}

// Упрощённая процедура "по ГОСТ": генерируем q (короткий простой), затем ищем p = q*k + 1 нужной длины
GenResult generate_by_gost_like(uint32_t bits, uint32_t need, std::mt19937_64 &rng, const vector<uint16_t> &small_primes, uint32_t qbits) {
    GenResult res;
    // генерируем q (короткий простой)
    auto gen_q = [&](uint32_t qb)->uint64_t {
        uint32_t attempts = 0;
        while (++attempts < 10000) {
            uint64_t cand = random_bits_uint(qb, rng);
            if (miller_rabin_probable(cand, 10, rng, small_primes)) return cand;
        }
        return 0;
    };
    uint64_t q = gen_q(qbits);
    if (q == 0) {
        // не удалось найти q — попробуем меньшую длину
        q = gen_q(std::max<uint32_t>(3, qbits/2));
        if (q == 0) return res;
    }
    // теперь ищем k так, чтобы p = q*k + 1 имела длину bits и была простой
    // границы k
    if (bits >= 63) return res; // ограничение по сдвигу
    uint64_t minP = (uint64_t(1) << (bits - 1));
    uint64_t maxP = (bits == 64) ? ~uint64_t(0) : ((uint64_t(1) << bits) - 1);
    uint64_t kmin = (minP + q - 1) / q;
    uint64_t kmax = maxP / q;
    if (kmin > kmax) return res;
    std::uniform_int_distribution<uint64_t> kdist(kmin, kmax);
    uint32_t attempts = 0;
    while (res.found.size() < need && attempts < 200000) {
        ++attempts;
        uint64_t k = kdist(rng);
        uint64_t p = q * k + 1;
        if (p < minP || p > maxP) continue;
        bool divisible = false;
        for (uint16_t sp : small_primes) {
            if (p == sp) { divisible = false; break; }
            if (p % sp == 0) { divisible = true; break; }
        }
        if (divisible) {
            if (miller_rabin_probable(p, 2, rng, small_primes)) ++res.false_rejected_by_method_but_probably_prime;
            continue;
        }
        if (miller_rabin_probable(p, 10, rng, small_primes)) {
            res.found.push_back(p);
        } else {
            if (miller_rabin_probable(p, 2, rng, small_primes)) ++res.false_rejected_by_method_but_probably_prime;
        }
    }
    return res;
}

// Помощник для печати таблицы результатов
void print_results_table(const string &title, const vector<uint64_t> &vals, const vector<char> &results, uint32_t k) {
    cout << "---- " << title << " ----\n";
    uint32_t n = vals.size();
    cout << std::setw(6) << "№";
    for (uint32_t i = 0; i < n; ++i) cout << std::setw(22) << (i+1);
    cout << "\n";
    cout << std::setw(6) << "p";
    for (uint32_t i = 0; i < n; ++i) cout << std::setw(22) << vals[i];
    cout << "\n";
    cout << std::setw(6) << "MR";
    for (uint32_t i = 0; i < n; ++i) cout << std::setw(22) << results[i];
    cout << "\n";
    cout << "k (rejected by method but MR says probable prime): " << k << "\n\n";
}

void calculatePrimeNumbers() {
    uint32_t bitlen = 32; // по умолчанию
    const uint32_t need = 10;
    // Случайный генератор
    std::random_device rd;
    std::mt19937_64 rng(rd());

    // 1) Решето до 500
    auto small_primes = sieve_primes_below(500);
    cout << "=== Простые числа < 500 (решето Эратосфена) ===\n";
    // печатаем по 10 в строке
    for (size_t i = 0; i < small_primes.size(); ++i) {
        cout << std::setw(4) << small_primes[i];
        if ((i+1) % 10 == 0) cout << "\n";
    }
    cout << "\n\n";

    // Генерации
    // 1a) Миллер
    uint32_t rounds_gen = 10; // для генерации более строгая проверка
    uint32_t rounds_check = 2; // для окончательной проверки, т.к. (1/4)^2 < 0.1
    cout << "Генерация " << need << " простых чисел длины " << bitlen << " бит.\n\n";

    auto g_miller = generate_by_miller(bitlen, need, rng, small_primes, rounds_gen);
    // финальная проверка всех найденных чисел MR с rounds_check
    vector<char> res_miller;
    for (auto p : g_miller.found) {
        bool ok = miller_rabin_probable(p, rounds_check, rng, small_primes);
        res_miller.push_back(ok ? '+' : '-');
    }
    print_results_table("Миллер-генератор", g_miller.found, res_miller, g_miller.false_rejected_by_method_but_probably_prime);

    // 1b) Поклингтон
    auto g_pock = generate_by_pocklington(bitlen, need, rng, small_primes);
    vector<char> res_pock;
    for (auto p : g_pock.found) {
        bool ok = miller_rabin_probable(p, rounds_check, rng, small_primes);
        res_pock.push_back(ok ? '+' : '-');
    }
    print_results_table("Поклингтон-генератор", g_pock.found, res_pock, g_pock.false_rejected_by_method_but_probably_prime);

    // 1c) ГОСТ-подобный (упрощённый)
    uint32_t qbits = std::max<uint32_t>(8, bitlen / 4);
    auto g_gost = generate_by_gost_like(bitlen, need, rng, small_primes, qbits);
    vector<char> res_gost;
    for (auto p : g_gost.found) {
        bool ok = miller_rabin_probable(p, rounds_check, rng, small_primes);
        res_gost.push_back(ok ? '+' : '-');
    }
    print_results_table("ГОСТ-образный генератор", g_gost.found, res_gost, g_gost.false_rejected_by_method_but_probably_prime);

    cout << "Примечание: rounds_check = " << rounds_check << " (вероятность ошибки ≤ 0.1).\n";
    cout << "Ограничения: поддерживаются длины бит ≤ 63. Для реальной криптографии требуется использование больших размеров и проверенных реализаций.\n";
}
