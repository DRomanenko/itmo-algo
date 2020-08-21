#include <iostream>
#include <string>
#include <vector>

typedef long long ll;

template<typename T>
class SieveOfEratosthenes {
 private:
    std::vector<bool> prime;
 public:
    SieveOfEratosthenes() = default;

    explicit SieveOfEratosthenes(const size_t &size) : prime(size + 1, true) {}

    void findAllPrime() {
        prime[0] = prime[1] = false;
        for (T i = 2; i < prime.size(); ++i)
            if (prime[(size_t) i]) {
                if (i * i < prime.size()) {
                    for (T q = i * i; q < prime.size(); q += i)
                        prime[(size_t) q] = false;
                }
            }
    }

    bool isPrime(const size_t &number) const {
        return prime[number];
    }
};

const std::string FILE_NAME = "test";

signed main() {
    // freopen((FILE_NAME + ".in").c_str(), "r", stdin);
    // freopen((FILE_NAME + ".out").c_str(), "w", stdout);
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr), std::cout.tie(nullptr);
    SieveOfEratosthenes<ll> sieveOfEratosthenes((size_t) 2e7);
    sieveOfEratosthenes.findAllPrime();
    size_t n;
    std::cin >> n;
    for (size_t i = 0, number; i < n; ++i) {
        std::cin >> number;
        std::cout << (sieveOfEratosthenes.isPrime(number) ? "YES" : "NO") << '\n';
    }
    return 0;
}
