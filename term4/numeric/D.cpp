#include <iostream>
#include <string>
#include <vector>
#include <random>

typedef long long ll;

ll MOD = INT64_MAX;

ll bin_mul(const ll &num1, const ll &num2) {
    ll a = num1, b = num2;
    ll res = 0;
    while (b) {
        if (b & 1) res = (res + a) % MOD;
        a = (a + a) % MOD;
        b >>= 1;
    }
    return res;
}

ll bin_pow(const ll &number, const ll &power) {
    ll a = number, n = power;
    ll res = 1;
    while (n) {
        if (n & 1) res = bin_mul(res, a);
        a = bin_mul(a, a);
        n >>= 1;
    }
    return res;
}

enum Colors {
    NO,
    YES
};

bool miller_rabin_primality_test(const ll &n) {
    if (n == 2) return YES;
    if (n == 1) return NO;

    ll s = n - 1, k = 0;
    while (!(s & 1)) {
        ++k;
        s >>= 1;
    }

    const size_t NUMBER_OF_CHECKS = 10;
    for (size_t i = 0; i < NUMBER_OF_CHECKS; ++i) {
        ll y = bin_pow(2 + rand() % (n - 2), s);

        if (y == 1 || y == n - 1)
            continue;

        for (size_t q = 0; q < k; ++q) {
            y = bin_mul(y, y);
            if (y == 1)
                return NO;
            if (y == n - 1) {
                break;
            }
        }
        if (y != n - 1)
            return NO;
    }
    return YES;
}

const std::string FILE_NAME = "test";

signed main() {
    // freopen((FILE_NAME + ".in").c_str(), "r", stdin);
    // freopen((FILE_NAME + ".out").c_str(), "w", stdout);
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr), std::cout.tie(nullptr);
    size_t n;
    std::cin >> n;
    for (size_t i = 0; i < n; ++i) {
        ll number;
        std::cin >> number;
        MOD = number;
        std::cout << (miller_rabin_primality_test(number) ? "YES" : "NO") << '\n';
    }
    return 0;
}
