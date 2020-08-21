#include <iostream>
#include <string>
#include <vector>

typedef long long ll;

template<typename T>
class SieveOfEratosthenes {
 private:
    std::vector<T> primes;

 public:
    SieveOfEratosthenes() = default;

    void findAllPrime(const size_t &size) {
        std::vector<bool> prime(size + 1, true);
        prime[0] = prime[1] = false;
        for (size_t i = 2; i < prime.size(); ++i)
            if (prime[(size_t) i]) {
                if (i * i < prime.size()) {
                    for (size_t q = i * i; q < prime.size(); q += i)
                        prime[(size_t) q] = false;
                }
                primes.push_back(i);
            }
    }

    int calc_hash(const size_t &n, const size_t &x) {
        int h = 0, k = 0, block_size = 1e5;
        std::vector<bool> prime(block_size, true);
        while (true) {
            for (auto cur_prime : primes) {
                int mod = block_size * k + 2, start = cur_prime - mod % cur_prime;
                for (int q = start + (((start + mod) / cur_prime == 1) ? cur_prime : 0);
                     q < block_size; q += cur_prime)
                    prime[q] = false;
            }
            for (int i = 0; i < block_size; ++i) {
                int cur_size = block_size * k + 2 + i;
                if (cur_size > (int) n) return h;
                if (prime[i]) h = h * (int) x + cur_size;
            }
            prime.assign((size_t) block_size, true);
            ++k;
        }
        return h;
    }
};

const std::string FILE_NAME = "test";

signed main() {
    // freopen((FILE_NAME + ".in").c_str(), "r", stdin);
    // freopen((FILE_NAME + ".out").c_str(), "w", stdout);
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr), std::cout.tie(nullptr);
    SieveOfEratosthenes<int> sieveOfEratosthenes;
    sieveOfEratosthenes.findAllPrime((size_t) 1e5);
    size_t n, x;
    std::cin >> n >> x;
    std::cout << sieveOfEratosthenes.calc_hash(n, x);
    return 0;
}
