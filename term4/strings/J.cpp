#include <iostream>
#include <vector>
 
using namespace std;
 
typedef unsigned long long ull;
 
const ull MOD = UINT64_MAX, PRIME = 239;  // дань уважения
 
vector<ull> x, prefixHash;
 
void precalc_x(const size_t &n) {
    x.resize(n);
    x[0] = 1;
    for (size_t i = 1; i < n; ++i) {
        x[i] = (x[i - 1] * PRIME) % MOD;
    }
}
 
void precalc_hash(const string &t) {
    prefixHash.resize(t.size() + 1);
    prefixHash[0] = 0;
    for (size_t i = 1; i < t.size() + 1; ++i) {
        prefixHash[i] = ((prefixHash[i - 1] * PRIME) % MOD + t[i - 1]) % MOD;
    }
}
 
void precalc(const string &t) {
    precalc_x(t.size());
    precalc_hash(t);
}
 
ull hash_t(const size_t &l, const size_t &r) {  // [l;r)
    return (prefixHash[r] - (prefixHash[l] * x[r - l]) % MOD) % MOD;
}
 
/** Поиск подстроки (полиномиальное хеширование с предпосчётом за O(t.size())) */
vector<size_t> find_all_substr_with_hash(const string &s, const string &t) {
    // O(t.size() + s.size()); At worst, O(t.size() * s.size())
    vector<size_t> ans;
    if (s.size() < t.size()) {
        ull hashS = s[0];
        for (size_t i = 1; i < s.size(); ++i)
            hashS = ((hashS * PRIME) % MOD + s[i]) % MOD;
        precalc(t);
        for (size_t i = 0; i < t.size() - s.size() + 1; ++i) {  // O(t.size())
            if (hashS == hash_t(i, i + s.size())) {
                bool flag = true;
                for (size_t q = 0; q < s.size(); ++q) {
                    if (t[i + q] != s[q]) {  // O(s.size())
                        flag = false;
                        break;
                    }
                }
                if (flag) {
                    ans.emplace_back(i + 1);
                }
            }
        }
    }
    return ans;
}
 
typedef long long ll;
 
/** Построение префикс функции */
vector<ll> precalc_prefix_func(const string &s) {  // O(n)
    vector<ll> prefixFunc(s.size() + 1, 0);
    prefixFunc[0] = -1;  // Префикс функция от пустой строки равна -1,
    // Поскольку к ней можно приделать любой символ и получится пустая строка.
    for (size_t i = 1; i < s.size() + 1; ++i) {  // O(n)
        // Хотим вязть все такие k, что они являются префиксом и суффиксом строки s[i-1].
        ll k = prefixFunc[i - 1];  // k увеличится на 1 s.size() раз
        // длина префикса
        // Переберем все k
        while (k >= 0) {  // Пока есть разумная строка p
            if (s[(size_t) k] == s[i - 1]) {  // Проверим, что строка нам подходит
                prefixFunc[i] = k + 1;
                break;
            }
            k = prefixFunc[(size_t) k];  // по сути берем префикс функцию от строки p
            // уменьшаяется <= n раз
        }
    }
    return prefixFunc;
}
 
/** Алгоритм Кнута-Морриса-Пратта */
vector<size_t> find_all_substr_with_kmp(const string &s, const string &t) {
    // O(t.size() + s.size())
    vector<size_t> ans;
    if (s.size() < t.size()) {
        string st = s + '#' + t;
        vector<ll> prefixFunc = precalc_prefix_func(st);
        for (size_t i = 0; i < prefixFunc.size(); ++i)
            if (prefixFunc[i] == s.size())
                ans.emplace_back(i - prefixFunc[i] - s.size());
    }
    return ans;
}
 
/** Построение Z-функции */
vector<ll> precalc_z_func(const string &s) {  // O(n)
    vector<ll> zFunc(s.size(), 0);
    zFunc[0] = 0;
    ll l = 0, r = 0;
    for (size_t i = 1; i < s.size(); ++i) {  // O(n)
        if (i + zFunc[i - (size_t) l] < r)
            zFunc[i] = max(0LL, zFunc[i - (size_t) l]);
        if (i + zFunc[i - (size_t) l] - 1 >= r) {
            zFunc[i] = max(0LL, r - (ll) i);
            while (i + zFunc[i] < s.size() &&
                   s[i + (size_t) zFunc[i]] == s[(size_t) zFunc[i]])  // увеличиваем r; r <= n
                ++zFunc[i];
            l = i, r = i + zFunc[i] - 1;
        }
        /** Short Version */
        /*
        zFunc[i] = max(0LL, min(zFunc[i - l], r - (ll) i));
        while (i + zFunc[i] < s.size() && s[i + zFunc[i]] == s[zFunc[i]])
            ++zFunc[i];
        if (i + zFunc[i] > r)
            l = i, r = i + zFunc[i];
        */
    }
    return zFunc;
}
 
string rev(string &s) {
    return string(s.rbegin(), s.rend());
}
 
/** Поиск подстроки в строке с одним несовпадением */
vector<size_t> find_all_substr_with_Error(string &s, string &t) {
    // O(t.size() + s.size())
    vector<size_t> ans;
    if (s.size() < t.size()) {
        vector<ll> zFunc = precalc_z_func(s + '#' + t),
                revZFunc = precalc_z_func(rev(s) + '#' + rev(t));
        // Нарисуй, чтобы вспомнить
        for (size_t i = 0; i < zFunc.size() - 2 * s.size(); ++i)
            if (zFunc[s.size() + 1 + i] + revZFunc[revZFunc.size() - s.size() - i] + 1 >= s.size())
                ans.emplace_back(i + 1);
    }
    return ans;
}
 
/** Построение префикс функции */
ll calc_prefix_func_and_find_max(const string &s) {  // O(n)
    ll max_k = 0;
    vector<ll> prefixFunc(s.size() + 1, 0);
    prefixFunc[0] = -1;
    for (size_t i = 1; i < s.size() + 1; ++i) {  // O(n)
        ll k = prefixFunc[i - 1];
        while (k >= 0) {
            if (s[(size_t) k] == s[i - 1]) {
                prefixFunc[i] = k + 1;
                max_k = max(max_k, k + 1);
                break;
            }
            k = prefixFunc[(size_t) k];
        }
    }
    return max_k;
}
 
const string FILE_NAME = "test";
 
int main() {
    // freopen((FILE_NAME + ".in").c_str(), "r", stdin);
    // freopen((FILE_NAME + ".out").c_str(), "w", stdout);
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    string s, pref;
    cin >> s;
    ull ans = 0;
    for (size_t i = 0; i < s.size(); ++i) {
        pref = s[i] + pref;
        ans += i + 1 - calc_prefix_func_and_find_max(pref);
        cout << ans << '\n';
    }
    return 0;
}