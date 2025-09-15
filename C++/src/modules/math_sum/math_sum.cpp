#include <bits/stdc++.h>
#include <cstdint>
#include <numeric>
using namespace std;

// parse simple rational from input string like "3", "2.5" or "7/2"
// returns false on parse error or if numerator/denom exceed limits
bool parse_rational(const string &s_in, uint64_t &num, uint64_t &den) {
	string s; // trimmed
	for (char c : s_in)
		if (!isspace((unsigned char)c))
			s.push_back(c);
	if (s.empty())
		return false;

	// fraction form "p/q"
	auto pos_slash = s.find('/');
	if (pos_slash != string::npos) {
		string p = s.substr(0, pos_slash);
		string q = s.substr(pos_slash + 1);
		if (p.empty() || q.empty())
			return false;
		// only digits allowed
		for (char c : p)
			if (!isdigit((unsigned char)c))
				return false;
		for (char c : q)
			if (!isdigit((unsigned char)c))
				return false;
		try {
			num = stoull(p);
			den = stoull(q);
		} catch (...) {
			return false;
		}
		if (den == 0)
			return false;
		return true;
	}

	// decimal form "123", "2.50", "0.333"
	auto pos_dot = s.find('.');
	if (pos_dot == string::npos) {
		// integer
		for (char c : s)
			if (!isdigit((unsigned char)c))
				return false;
		try {
			num = stoull(s);
			den = 1;
		} catch (...) {
			return false;
		}
		return true;
	} else {
		string intpart = s.substr(0, pos_dot);
		string fracpart = s.substr(pos_dot + 1);
		if (intpart.empty())
			intpart = "0";
		// digits check
		for (char c : intpart)
			if (!isdigit((unsigned char)c))
				return false;
		for (char c : fracpart)
			if (!isdigit((unsigned char)c))
				return false;
		// trim trailing zeros in fracpart
		while (!fracpart.empty() && fracpart.back() == '0')
			fracpart.pop_back();
		if (fracpart.empty()) {
			// effectively integer
			try {
				num = stoull(intpart);
				den = 1;
			} catch (...) {
				return false;
			}
			return true;
		}
		// limit decimal places to prevent huge denominators
		if (fracpart.size() > 12)
			return false; // safety limit
		string full = intpart + fracpart;
		try {
			num = stoull(full);
		} catch (...) {
			return false;
		}
		den = 1;
		for (size_t i = 0; i < fracpart.size(); ++i)
			den *= 10ULL;
		// reduce
		uint64_t g = std::gcd(num, den);
		num /= g;
		den /= g;
		return true;
	}
}

// safe pow for uint64_t with overflow check
bool pow_u64(uint64_t base, unsigned exp, uint64_t &out) {
	uint64_t r = 1;
	for (unsigned i = 0; i < exp; ++i) {
		if (base != 0 && r > UINT64_MAX / base)
			return false;
		r *= base;
	}
	out = r;
	return true;
}

void calculateMathSum() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	string sa, sb;
	if (!(cin >> sa >> sb))
		return;

	uint64_t a_num, a_den, b_num, b_den;
	if (!parse_rational(sa, a_num, a_den) || !parse_rational(sb, b_num, b_den)) {
		cout << "irrational\n";
		return;
	}

	// check range 1 <= value <= 10  〈=>〉 num/den in [1,10]
	// check num >= den and num <= 10*den (watch overflow)
	if (a_num < a_den) {
		cout << "irrational\n";
		return;
	}
	if (b_num < b_den) {
		cout << "irrational\n";
		return;
	}
	// check a_num <= 10*a_den safely
	if (a_num > 10ULL * a_den) {
		cout << "irrational\n";
		return;
	}
	if (b_num > 10ULL * b_den) {
		cout << "irrational\n";
		return;
	}

	// if b == 1 -> divergence
	if (b_num == b_den) {
		cout << "infinity\n";
		return;
	}

	// require b > 1
	if (b_num <= b_den) {
		cout << "infinity\n";
		return;
	}

	// if 'a' is integer (den==1) and small (<=10) we can produce exact rational
	if (a_den == 1) {
		uint64_t m = a_num;
		if (m == 0) { // sum_{n>=1} 1/b^n = 1/(b-1)  (not in original range but safety)
			// compute exact rational: numerator = 1, denominator = b_num - b_den divided by gcd
			uint64_t numer = 1;
			// denominator = (b_num - b_den)
			uint64_t denom = b_num - b_den;
			uint64_t g = std::gcd(numer, denom);
			numer /= g;
			denom /= g;
			cout << numer << "/" << denom << "\n";
			return;
		}
		if (m > 10) {
			cout << "irrational\n";
			return;
		} // out of problem spec

		// compute Eulerian numbers A(m,k) via recurrence
		vector<vector<uint64_t>> A(m + 1, vector<uint64_t>(m + 1, 0));
		A[0][0] = 1;
		for (uint64_t n = 1; n <= m; ++n) {
			for (uint64_t k = 0; k <= n; ++k) {
				uint64_t val = 0;
				if (k > 0) {
					uint64_t t = (n - k) * A[n - 1][k - 1];
					val += t;
				}
				if (k <= n - 1) {
					uint64_t t = (k + 1) * A[n - 1][k];
					val += t;
				}
				A[n][k] = val;
			}
		}

		// b = p/q where p=b_num, q=b_den; x = 1/b = q/p
		uint64_t p = b_num, q = b_den;

		// compute numerator S = sum_{k=0}^m A(m,k) * q^{k+1} * p^{m-k}
		uint64_t S = 0;
		for (uint64_t k = 0; k <= m; ++k) {
			uint64_t a_coeff = A[m][k];
			uint64_t pow_q, pow_p;
			if (!pow_u64(q, (unsigned)(k + 1), pow_q)) {
				cout << "irrational\n";
				return;
			}
			if (!pow_u64(p, (unsigned)(m - k), pow_p)) {
				cout << "irrational\n";
				return;
			}
			// term = a_coeff * pow_q * pow_p  (check overflow stepwise)
			if (a_coeff != 0 && pow_q > UINT64_MAX / a_coeff) {
				cout << "irrational\n";
				return;
			}
			uint64_t t = a_coeff * pow_q;
			if (pow_p != 0 && t > UINT64_MAX / pow_p) {
				cout << "irrational\n";
				return;
			}
			uint64_t term = t * pow_p;
			if (S > UINT64_MAX - term) {
				cout << "irrational\n";
				return;
			}
			S += term;
		}

		// denom = (p - q)^{m+1}
		uint64_t base = p - q;
		uint64_t D;
		if (!pow_u64(base, (unsigned)(m + 1), D)) {
			cout << "irrational\n";
			return;
		}

		// reduce fraction
		uint64_t g = std::gcd(S, D);
		S /= g;
		D /= g;
		cout << S << "/" << D << "\n";
		return;
	}

	// otherwise we treat as non-integer exponent => general polylog value -> declare irrational
	cout << "irrational\n";
	return;
}
