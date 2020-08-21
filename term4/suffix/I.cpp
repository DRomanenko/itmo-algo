#include <iostream>
#include <string>
#include <vector>
#include <map>
 
/** Суффиксный автомат (англ. Suffix Automaton, directed acyclic word graph) — минимальный ДКА,
 * который принимает все суффиксы строки и только их.
 * http://gg.gg/htsnp
*/
class SuffixAutomaton {
 private:
    struct Node {
        int link{}, len{};
        std::map<char, int> children;
 
        Node() = default;
 
        Node(const int &link, const int &len,
             std::map<char, int> children) : link(link),
                                             len(len),
                                             children(std::move(children)) {}
    };
 
 public:
    std::vector<Node> tree;
    int sz, prev;
 
    SuffixAutomaton() {
        sz = 1, prev = 0;
        tree.emplace_back();
        tree[0] = {-1, 0, {}};
    }
 
    void add_s(char c) {
        int curr_sz = sz++;
        tree.emplace_back();
        tree[curr_sz].len = tree[prev].len + 1;
        int temp = prev;
        for (; temp != -1 && !tree[temp].children.count(c); temp = tree[temp].link) {
            tree[temp].children[c] = curr_sz;
        }
        if (temp == -1) {
            tree[curr_sz].link = 0;
        } else {
            int child = tree[temp].children[c];
            if (tree[child].len == tree[temp].len + 1) {
                tree[curr_sz].link = child;
            } else {
                int clone = sz++;
                tree.emplace_back();
                tree[clone] = {tree[child].link, tree[temp].len + 1,
                               tree[child].children};
                tree[child].link = clone;
                tree[curr_sz].link = clone;
 
                for (; temp != -1 && tree[temp].children.count(c) &&
                       tree[temp].children[c] == child;
                       temp = tree[temp].link) {
                    tree[temp].children[c] = clone;
                }
            }
        }
        prev = curr_sz;
    }
};
 
using namespace std;
 
const std::string FILE_NAME = "test";
 
signed main() {
    // freopen((FILE_NAME + ".in").c_str(), "r", stdin);
    // freopen((FILE_NAME + ".out").c_str(), "w", stdout);
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    string s;
    auto *text = new SuffixAutomaton();
    while (getline(cin, s)) {
        if (s[0] == 'A') {
            for (size_t i = 2; i < s.length(); ++i)
                text->add_s((char) tolower(s[i]));
        } else {
            bool flag = true;
            for (int i = 2, index = 0; i < (int) s.length(); ++i) {
                if (!text->tree[index].children.count((char) tolower(s[i]))) {
                    flag = false;
                    break;
                }
                index = text->tree[index].children[(char) tolower(s[i])];
            }
            cout << (flag ? "YES" : "NO") << '\n';
        }
    }
    return 0;
}