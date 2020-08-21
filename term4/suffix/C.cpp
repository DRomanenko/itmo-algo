#include <iostream>
#include <unordered_map>
#include <queue>
#include <stack>
#include <map>
#include <algorithm>
 
using namespace std;
 
const long long INF = 9223372036854775807LL;
 
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
 
        Node(size_t number, int start_pos, int end_pos, size_t depth, Node *parent) :
                number(number), start_pos(start_pos), end_pos(end_pos), depth(depth),
                parent(parent) {}
 
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
};
 
const string FILE_NAME = "test";
 
int main() {
    // freopen((FILE_NAME + ".in").c_str(), "r", stdin);
    // freopen((FILE_NAME + ".out").c_str(), "w", stdout);
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    string s;
    cin >> s;
 
    CompressedSuffixTrie *compressedSuffixTrie = new CompressedSuffixTrie(s);
    compressedSuffixTrie->print_ans("bfs");
    delete compressedSuffixTrie;
    return 0;
}