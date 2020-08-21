#include <iostream>
#include <unordered_map>
#include <queue>
 
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
 
const string FILE_NAME = "test";
 
int main() {
    // freopen((FILE_NAME + ".in").c_str(), "r", stdin);
    // freopen((FILE_NAME + ".out").c_str(), "w", stdout);
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    string s;
    cin >> s;
    SuffixTrie *suffixTrie = new SuffixTrie(s);
    suffixTrie->print_ans("dfs");
    delete suffixTrie;
    return 0;
}