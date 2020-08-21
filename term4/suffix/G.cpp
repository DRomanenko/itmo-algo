#include <iostream>
#include <unordered_map>
#include <queue>
#include <stack>
#include <map>
#include <algorithm>
#include <numeric>
#include <set>
 
using namespace std;
 
const long long INF = 9223372036854775807LL;
typedef long long ll;
typedef unsigned long long ull;
 
/** Суффиксный бор (англ. Suffix Trie) — бор, содержащий все суффиксы данной строки.
 * https://www.youtube.com/watch?v=BP9LXwosFco
 * http://gg.gg/gy0ru
*/
class SuffixTrie {
 private:
    struct Node {
        size_t number;
        unordered_map<char, Node *> children{};
 
        explicit Node(size_t number = 0) : number(number) {}
    };
 
    string data;
    Node *root;
    size_t qty_edges;
 
    void bfs_print_trie() {
        queue<Node *> q;
        q.push(root);
        while (!q.empty()) {
            Node *current = q.front();
            q.pop();
            if (!current->children.empty()) {
                for (char symbol = 'a'; symbol <= 'z'; ++symbol) {
                    if (current->children.find(symbol) != current->children.end()) {
                        cout << current->number + 1 << ' '
                             << current->children[symbol]->number + 1 << ' '
                             << symbol << '\n';
                        q.push(current->children[symbol]);
                    }
                }
            }
        }
    }
 
    void dfs_print_trie(Node *current) {
        if (!current->children.empty()) {
            for (char symbol = 'a'; symbol <= 'z'; ++symbol) {
                if (current->children.find(symbol) != current->children.end()) {
                    cout << current->number + 1 << ' '
                         << current->children[symbol]->number + 1 << ' '
                         << symbol << '\n';
                    dfs_print_trie(current->children[symbol]);
                }
            }
        }
    }
 
 public:
    explicit SuffixTrie(const string &data) : data(data),
                                              root(new Node()),
                                              qty_edges(0) {
        const size_t data_size = this->data.size();
        for (size_t i = 0; i < data_size; ++i) {
            insert(i, data_size);
        }
    }
 
    void insert(size_t start_pos, size_t end_pos) {  // [start_pos, end_pos)
        Node *current = root;
        for (size_t i = start_pos; i < end_pos; ++i) {
            char symbol = data[i];
            if (current->children.find(symbol) == current->children.end()) {
                current->children[symbol] = new Node(++qty_edges);
            }
            current = current->children[symbol];
        }
    }
 
    void print_ans(const string &mod) {
        cout << qty_edges + 1 << ' ' << qty_edges << '\n';
        print_trie(mod);
    }
 
    void print_trie(const string &mod) {
        if (mod == "dfs") {
            dfs_print_trie(root);
        } else if (mod == "bfs") {
            bfs_print_trie();
        }
    }
};
 
size_t sz1, sz2;
 
/** Суффиксное дерево (англ. Suffix Tree) aka Сжатый суффиксный бор (англ. Compressed Suffix Trie) aka — сжатый бор, содержащий все суффиксы данной строки или же все подстроки строки s.
 * https://www.youtube.com/watch?v=LNBs3xZMGLc
 * Суффиксное дерево:                       http://gg.gg/gy277
 * Построение Алгоритмом Укконена за O(n):  http://gg.gg/gy24d
 * Код:                                     https://sites.google.com/site/indy256/algo/suffix_tree
 * http://gg.gg/gy0ru
*/
class CompressedSuffixTrie {
 private:
    struct Node {
        size_t number;
        int start_pos, end_pos;
        size_t depth;
        Node *parent;
        map<char, Node *> children{};
        Node *suf_link = nullptr;
        bool is1, is2;
 
        Node(size_t number, int start_pos, int end_pos, size_t depth, Node *parent) :
                number(number), start_pos(start_pos), end_pos(end_pos), depth(depth),
                parent(parent), is1(false), is2(false) {}
 
        bool check_null(const Node *n1, const Node *n2) const {
            return ((n1 == nullptr && n2 == nullptr) ||
                    (n1 != nullptr && n2 != nullptr && n1 == n2));
        }
 
        bool operator==(const Node &other) const {
            return start_pos == other.start_pos &&
                   depth == other.depth &&
                   children == other.children &&
                   check_null(this->parent, other.suf_link) &&
                   check_null(this->suf_link, other.suf_link);
        }
    };
 
    string data;
    Node *root;
    size_t qty_edges;
 
    void bfs_print_tree() {
        queue<Node *> q;
        q.push(root);
        while (!q.empty()) {
            Node *current = q.front();
            q.pop();
            if (!current->children.empty()) {
                for (char symbol = 'a'; symbol <= 'z'; ++symbol) {
                    if (current->children.find(symbol) != current->children.end()) {
                        cout << current->number + 1 << ' '
                             << current->children[symbol]->number + 1 << ' '
                             << current->children[symbol]->start_pos + 1 << ' '
                             << current->children[symbol]->end_pos << '\n';
                        q.push(current->children[symbol]);
                    }
                }
            }
        }
    }
 
    void dfs_print_tree(Node *current) {
        if (!current->children.empty()) {
            for (char symbol = 'a'; symbol <= 'z'; ++symbol) {
                if (current->children.find(symbol) != current->children.end()) {
                    cout << current->number + 1 << ' '
                         << current->children[symbol]->number + 1 << ' '
                         << current->children[symbol]->start_pos + 1 << ' '
                         << current->children[symbol]->end_pos << '\n';
                    dfs_print_tree(current->children[symbol]);
                }
            }
        }
    }
 
    ll count_substr(Node *current) {
        if (!current->children.empty()) {
            ll ans = 0LL;
            for (char symbol = 'a'; symbol <= 'z'; ++symbol) {
                if (current->children.find(symbol) != current->children.end()) {
                    int curr_len = current->children[symbol]->end_pos -
                                   current->children[symbol]->start_pos;
                    ans += count_substr(current->children[symbol]) + curr_len;
                }
            }
            return ans;
        } else {
            return 0LL;
        }
    }
 
    /* ll c_substr(Node *current) {
        if (!current->children.empty()) {
            ll ans = 0LL;
            for (char symbol = 'a'; symbol <= 'z'; ++symbol) {
                if (current->children.find(symbol) != current->children.end()) {
                    if (current->children[symbol]->start_pos < ()sz1) {
                        current->children[symbol]->is1 = true;
                    } else {
                        current->children[symbol]->is2 = true;
                    }
                }
            }
            return ans;
        } else {
            return 0LL;
        }
    }*/
 
 public:
    explicit CompressedSuffixTrie(const string &data) : data(data),
                                                        root(new Node(0,
                                                                      0, 0,
                                                                      0, nullptr)),
                                                        qty_edges(0) {
        Node *ans = root;
        size_t number = 0;
        for (int i = 0, tail = 0; i < (int) this->data.size(); ++i, ++tail) {
            Node *last = nullptr;
            while (tail >= 0) {
                Node *current = ans->children[data[i - tail]];
                while (current != nullptr && tail >= current->end_pos - current->start_pos) {
                    tail -= current->end_pos - current->start_pos;
                    ans = current;
                    current = current->children[data[i - tail]];
                }
                if (current == nullptr) {
                    ans->children[data[i]] =
                            new Node(++number,
                                     i,
                                     this->data.size(),
                                     ans->depth + ans->end_pos - ans->start_pos,
                                     ans);
                    ++qty_edges;
                    if (last != nullptr) {
                        last->suf_link = ans;
                    }
                    last = nullptr;
                } else {
                    char save = data[current->start_pos + tail];
                    if (save == data[i]) {
                        if (last != nullptr) {
                            last->suf_link = ans;
                        }
                        break;
                    } else {
                        Node *split_ans =
                                new Node(++number,
                                         current->start_pos, current->start_pos + tail,
                                         ans->depth + ans->end_pos - ans->start_pos, ans);
                        split_ans->children[data[i]] =
                                new Node(++number,
                                         i, data.size(),
                                         current->depth + tail,
                                         split_ans);
                        qty_edges += 2;
                        split_ans->children[save] = current;
                        current->start_pos += tail;
                        current->depth += tail;
                        current->parent = split_ans;
                        ans->children[data[i - tail]] = split_ans;
                        if (last != nullptr) {
                            last->suf_link = split_ans;
                        }
                        last = split_ans;
                    }
                }
                if (*ans == *root) {
                    --tail;
                } else {
                    ans = ans->suf_link;
                }
            }
        }
    }
 
    void print_ans(const string &mod) {
        cout << qty_edges + 1 << ' ' << qty_edges << '\n';
        print_tree(mod);
    }
 
    void print_tree(const string &mod) {
        if (mod == "dfs") {
            dfs_print_tree(root);
        } else if (mod == "bfs") {
            bfs_print_tree();
        }
    }
 
    void print_count_substr() {
        cout << count_substr(root) << '\n';
    }
};
 
/** Суффиксный массив (англ. Suffix Array) - перестановка индексов суффиксов, которая задаёт порядок суффиксов в порядке лексикографической сортировки.
 * https://www.youtube.com/watch?v=LNBs3xZMGLc
 * Суффиксный массив:                       http://gg.gg/hryj1
 * Алгоритм Касаи получения lcp:            http://gg.gg/hryjv
 * e-maxx:                                  https://e-maxx.ru/algo/suffix_array
*/
class SuffixArray {
 private:
    string data;
    vector<int> suff_arr;
    vector<int> lcp;
 
 public:
    explicit SuffixArray(const string &data) : data(data) {
        int sz = data.length();
 
        for (int i = sz - 1; i >= 0; --i) {
            suff_arr.push_back(i);
        }
        stable_sort(suff_arr.begin(), suff_arr.end(),
                    [&](int a, int b) { return data[a] < data[b]; });
 
        vector<int> classes(sz);
        for (int i = 0; i < sz; ++i) {
            classes[i] = data[i];
        }
        for (int len = 1; len < sz; len *= 2) {
            vector<int> c = classes;
            for (int i = 0; i < sz; i++)
                classes[suff_arr[i]] = i > 0 &&
                                       c[suff_arr[i - 1]] == c[suff_arr[i]] &&
                                       suff_arr[i - 1] + len < sz &&
                                       c[suff_arr[i - 1] + len / 2] == c[suff_arr[i] + len / 2]
                                       ? classes[suff_arr[i - 1]] : i;
 
            vector<int> cnt(sz);
            iota(cnt.begin(), cnt.end(), 0);
            vector<int> s = suff_arr;
            for (int i = 0; i < sz; i++) {
                int s1 = s[i] - len;
                if (s1 >= 0)
                    suff_arr[cnt[classes[s1]]++] = s1;
            }
        }
    }
 
    void print_array() {
        for (auto u : suff_arr)
            cout << u + 1 << ' ';
        cout << '\n';
    }
 
    void precalc_lcp() {
        int sz = data.size();
        vector<int> rank(sz);
        for (int i = 0; i < sz; ++i)
            rank[suff_arr[i]] = i;
        lcp.resize(sz);
        for (int i = 0, tmp = 0; i < sz; ++i) {
            if (rank[i] < sz - 1) {
                for (int j = suff_arr[rank[i] + 1];
                     data[i + tmp] == data[j + tmp];
                     ++tmp) {}
                lcp[rank[i]] = tmp;
                if (tmp > 0)
                    --tmp;
            }
        }
    }
 
    void print_lcp() {
        for (auto u : lcp)
            cout << u << ' ';
        cout << '\n';
    }
 
    int get_suff(int pos) {
        return suff_arr[pos];
    }
 
    int get_lcp(int pos) {
        return lcp[pos];
    }
 
    void ans_refren() {
        print_lcp();
        int sz = data.size();
        vector<vector<int>> v(sz);
        for (int i = 0; i < sz; ++i)
            v[lcp[i]].push_back(i);
        set<int> st;
        st.insert(-1);
        st.insert(sz);
        ll ans = sz, len = sz, curr_suff = 0;
        for (int i = 0; i < sz; ++i) {
            for (int q = 0; q < (int) v[i].size(); ++q) {
                ll x1 = *st.lower_bound(v[i][q]);
                ll x2 = *(--st.lower_bound(v[i][q]));
                ll p = (x1 - 1) - (x2 + 1) + 1;
                if (ans < (p + 1) * i) {
                    ans = (p + 1) * i;
                    len = i;
                    curr_suff = suff_arr[v[i][q]];
                }
            }
            for (auto u : v[i])
                st.insert(u);
        }
        cout << ans << '\n' << len << '\n';
        for (ll i = curr_suff; i < curr_suff + len; ++i)
            cout << data[(size_t) i] - 'a' + 1 << " ";
        cout << '\n';
    }
 
    void max_common_substr(int n) {
        precalc_lcp();
        size_t pos = 0, mx_len = 0;
        for (size_t i = 0; i < data.size(); ++i) {
            if (((suff_arr[i] < n && suff_arr[i - 1] > n) ||
                 (suff_arr[i] > n && suff_arr[i - 1] < n)) &&
                ((size_t)lcp[i - 1] > mx_len)) {
                pos = suff_arr[i];
                mx_len = lcp[i - 1];
            }
        }
        for (size_t i = pos; i < pos + mx_len; ++i) {
            if (data[i] != '\0') {
                cout << data[i];
            }
        }
        cout << '\n';
    }
};
 
const string FILE_NAME = "test";
 
signed main() {
    // freopen((FILE_NAME + ".in").c_str(), "r", stdin);
    // freopen((FILE_NAME + ".out").c_str(), "w", stdout);
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    string s, t;
    cin >> s >> t;
    SuffixArray *suffixArray = new SuffixArray(s + '\0' + t);
    suffixArray->max_common_substr(s.size());
    delete suffixArray;
    // CompressedSuffixTrie *compressedSuffixTrie
    // = new CompressedSuffixTrie(s1);//+ '\0' + s2 + '\1');
    // compressedSuffixTrie->print_ans("bfs");
    // compressedSuffixTrie->print_count_substr();
    // delete compressedSuffixTrie;
    return 0;
}