#include <iostream>
#include <vector>
#include <map>
#include <deque>
#include <set>
 
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
 
/** Алгоритм Ахо — Корасик **/
struct node {
    ll link, parent;
    char symbol;
    bool term, used_dfs, used_link;
 
    map<char, ll> childs;
    set<ll> id;
 
    explicit node(ll parent = -1, char ch = 0, bool term = false) :
            link(-1),
            parent(parent),
            symbol(ch),
            term(term),
            used_dfs(false),
            used_link(false),
            childs(map<char, ll>{}),
            id(set<ll>{}) {}
 
    ll find_child(char elem) {
        auto it = childs.find(elem);
        return (it == childs.end() ? -1 : it->second);
    }
 
    void add_child(char new_symbol, ll ind) {
        childs.insert({new_symbol, ind});
    }
 
    void add_id(ll ind) {
        id.insert(ind);
    }
};
 
struct corasick {
    vector<node> nodes;
    ll qty;
 
    corasick() : nodes(vector<node>{node()}),
                 qty(0LL) {}
 
    void build() {
        deque<ll> d{0};
        while (!d.empty()) {
            add_link(d.front());
            for (auto it : nodes[(size_t) d.front()].childs)
                d.push_back(it.second);
            d.pop_front();
        }
    }
 
    void add_pattern(string s) {
        size_t curr = 0;
        for (size_t i = 0; i < s.size(); ++i) {
            if (nodes[curr].find_child(s[i]) == -1) {
                nodes[curr].add_child(s[i], nodes.size());
                nodes.emplace_back(curr, s[i], i + 1 == s.size());
            }
            curr = (size_t) nodes[curr].find_child(s[i]);
        }
        nodes[curr].term = true;
        nodes[curr].add_id(qty++);
    }
 
    void add_link(ll ind) {
        if (ind != 0) {
            ll curr = (ll)nodes[(size_t)nodes[(size_t) ind].parent].link;
            while (curr != -1 && nodes[(size_t) curr].find_child(nodes[(size_t) ind].symbol) == -1)
                curr = nodes[(size_t) curr].link;
            nodes[(size_t) ind].link =
                    (curr == -1 ? 0 :
                     nodes[(size_t) curr].find_child(nodes[(size_t) ind].symbol));
        }
    }
 
    set<ll> find_match_in_text(string s) {
        set<ll> ans;
        size_t curr = 0, i = 0;
 
        while (i < s.size()) {
            curr = (size_t) next(s[i++], curr);
            nodes[curr].used_link = true;
        }
 
        for (size_t q = 0; q < nodes.size(); ++q)
            if (!nodes[q].used_dfs && nodes[q].used_link)
                dfs(q);
 
        for (size_t q = 1; q < nodes.size(); ++q) {
            if (nodes[q].used_dfs && nodes[q].term)
                ans.insert(nodes[q].id.begin(), nodes[q].id.end());
            nodes[q].used_link = false;
        }
        return ans;
    }
 
    ll next(char symbol, ll ind) {
        while (ind != -1 && nodes[(size_t) ind].find_child(symbol) == -1)
            ind = nodes[(size_t) ind].link;
        return (ind == -1 ? 0 : nodes[(size_t)ind].find_child(symbol));
    }
 
    void dfs(ll curr) {
        nodes[(size_t)curr].used_dfs = true;
        if (nodes[(size_t)curr].link != -1 && !nodes[(size_t)nodes[(size_t)curr].link].used_dfs)
            dfs(nodes[(size_t)curr].link);
    }
};
 
const string FILE_NAME = "test";
 
int main() {
    // freopen((FILE_NAME + ".in").c_str(), "r", stdin);
    // freopen((FILE_NAME + ".out").c_str(), "w", stdout);
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    size_t n;
    cin >> n;
    corasick finder;
    for (size_t i = 0; i < n; ++i) {
        string s;
        cin >> s;
        finder.add_pattern(s);
    }
    finder.build();
    string t;
    cin >> t;
    auto ans = finder.find_match_in_text(t);
    for (size_t i = 0; i < n; ++i)
        cout << (ans.find(i) != ans.end() ? "YES" : "NO") << '\n';
    return 0;
}