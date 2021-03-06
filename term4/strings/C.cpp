#include <iostream>
#include <vector>
 
using namespace std;
 
typedef unsigned long long ull;
 
const ull MOD = UINT64_MAX, PRIME = 1;
 
vector<ull> x, prefixHash;
 
void precalcX(const size_t &n) {
    x.resize(n);
    x[0] = 1;
    for (size_t i = 1; i < n; ++i) {
        x[i] = (x[i - 1] * PRIME) % MOD;
    }
}
 
void precalcHash(const string &t) {
    prefixHash.resize(t.size() + 1);
    prefixHash[0] = 0;
    for (size_t i = 1; i < t.size() + 1; ++i) {
        prefixHash[i] = ((prefixHash[i - 1] * PRIME) % MOD + t[i - 1]) % MOD;
    }
}
 
void precalc(const string &t) {
    precalcX(t.size());
    precalcHash(t);
}
 
ull hashT(const int &l, const int &r) {  // [l;r)
    return (prefixHash[r] - (prefixHash[l] * x[r - l]) % MOD) % MOD;
}
 
/** Поиск подстроки (полиномиальное хеширование с предпосчётом за O(t.size())) */
vector<size_t> findAllSubStrWithHash(string &s, string &t) {
    // O(t.size() + s.size()); At worst, O(t.size() * s.size())
    vector<size_t> ans;
    if (s.size() < t.size()) {
        ull hashS = s[0];
        for (size_t i = 1; i < s.size(); ++i)
            hashS = ((hashS * PRIME) % MOD + s[i]) % MOD;
        precalc(t);
        for (size_t i = 0; i < t.size() - s.size() + 1; ++i) {  // O(t.size())
            if (hashS == hashT(i, i + s.size())) {
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
 
vector<ll> precalcPrefixFunc(string &s) {  // O(n)
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
vector<size_t> findAllSubStrWithKMP(string &s, string &t) {
    // O(t.size() + s.size())
    vector<size_t> ans;
    if (s.size() < t.size()) {
        string st = s + '#' + t;
        vector<ll> prefixFunc = precalcPrefixFunc(st);
        for (size_t i = 0; i < prefixFunc.size(); ++i)
            if (prefixFunc[i] == s.size())
                ans.emplace_back(i - prefixFunc[i] - s.size());
    }
    return ans;
}
 
const string FILE_NAME = "test";
 
int main() {
    // freopen((FILE_NAME + ".in").c_str(), "r", stdin);
    // freopen((FILE_NAME + ".out").c_str(), "w", stdout);
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    string s;
    cin >> s;
    vector<ll> prefixFunc = precalcPrefixFunc(s);
    for (auto e : prefixFunc)
        if (e != -1)
            cout << e << ' ';
    return 0;
}