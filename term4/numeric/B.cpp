#include <iostream>
#include <string>
#include <vector>

typedef long long ll;
typedef unsigned long long ull;

template<typename T>
class SieveOfEratosthenes {
 private:
    std::vector<T> primes;
    std::vector<T> minimal_simple_divider;

 public:
    SieveOfEratosthenes() = default;

    // We assume all numbers are simple (for all i : minimal_simple_divider[i] == 0).
    explicit SieveOfEratosthenes(const size_t &size) : minimal_simple_divider(size + 1) {}

    void findAllPrime() {
        for (T i = 2; i < minimal_simple_divider.size(); ++i) {
            if (minimal_simple_divider[i] == 0) {
                minimal_simple_divider[i] = i;
                primes.push_back(i);
            }
            for (auto cur_prime : primes) {
                if (cur_prime > minimal_simple_divider[i] ||
                        i * 1LL * cur_prime > minimal_simple_divider.size())
                    break;
                minimal_simple_divider[i * cur_prime] = cur_prime;
            }
        }
    }

    std::vector<T> factorization(const T &number) const {
        T cur_number = number;
        std::vector<T> result;
        while (minimal_simple_divider[cur_number] != cur_number) {
            result.push_back(minimal_simple_divider[cur_number]);
            cur_number /= minimal_simple_divider[cur_number];
        }
        result.push_back(cur_number);
        return result;
    }
};

const std::string FILE_NAME = "test";

signed main() {
    // freopen((FILE_NAME + ".in").c_str(), "r", stdin);
    // freopen((FILE_NAME + ".out").c_str(), "w", stdout);
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr), std::cout.tie(nullptr);
    SieveOfEratosthenes<size_t> sieveOfEratosthenes((size_t) 1e6);
    sieveOfEratosthenes.findAllPrime();
    size_t n;
    std::cin >> n;
    for (size_t i = 0, number; i < n; ++i) {
        std::cin >> number;
        for (auto cur_divider : sieveOfEratosthenes.factorization(number))
            std::cout << cur_divider << ' ';
        std::cout << '\n';
    }
    return 0;
}
