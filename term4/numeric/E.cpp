#include <iostream>
#include <string>
#include <vector>

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

struct result {
    ll g, x, y;
};

result gcd(const ll &a, const ll &b) {
    if (b == 0) return {a, 1, 0};
    auto[g, x, y] = gcd(b, a % b);
    return {g, y, x - y * (a / b)};
}

ll get_inverse(const ll &e, const ll &phi) {
    auto[g, x, _] = gcd(e, phi);
    return x / g + (x < 0 ? phi : 0);
}

const std::string FILE_NAME = "test";

signed main() {
    // freopen((FILE_NAME + ".in").c_str(), "r", stdin);
    // freopen((FILE_NAME + ".out").c_str(), "w", stdout);
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr), std::cout.tie(nullptr);
    ll n, e, c;
    std::cin >> n >> e >> c;
    MOD = n;
    ll p = 0;
    for (ll i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            p = i;
            break;
        }
    }
    ll q = n / p, phi = (p - 1) * (q - 1), d = get_inverse(e, phi);
    std::cout << bin_pow(c, d) << '\n';
    return 0;
}
