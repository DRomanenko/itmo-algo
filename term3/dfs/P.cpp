#include <iostream>
#include <vector>
#include <stack>
#include <set>
 
using namespace std;
 
size_t MAXS = 300;
vector<set<size_t>> g(MAXS);
vector<set<size_t>> rg(MAXS);
vector<bool> used(MAXS);
vector<int> comp(MAXS);
stack<int> order;
 
void dfs1(size_t v) {
    used[v] = true;
    for (auto to : g[v])
        if (!used[to])
            dfs1(to);
    order.push(v);
}
 
void dfs2(size_t v, int c) {
    comp[v] = c;
    for (auto to : rg[v])
        if (!comp[to])
            dfs2(to, c);
}
 
int main() {
    freopen("cnf.in", "r", stdin);
    freopen("cnf.out", "w", stdout);
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int n, m;
    cin >> n >> m;
 
    for (size_t i = 0; (int)i < m; ++i) {
        int a, b, c, _a, _b, _c;
        cin >> _a >> _b >> _c;
        if (_a < 0) {
            ++_a;
            _a = -_a;
            a = _a + n;
        } else {
            a = --_a;
            _a += n;
        }
        if (_b < 0) {
            ++_b;
            _b = -_b;
            b = _b + n;
        } else {
            b = --_b;
            _b += n;
        }
        if (_c < 0) {
            ++_c;
            _c = -_c;
            c = _c + n;
        } else {
            c = --_c;
            _c += n;
        }
        g[_a].insert(b);  // !a -> b
        g[_b].insert(a);  // !b -> a
        g[_a].insert(c);  // !a -> c
        g[_c].insert(a);  // !c -> a
        g[_b].insert(c);  // !b -> c
        g[_c].insert(b);  // !c -> b
        rg[b].insert(_a);
        rg[a].insert(_b);
        rg[c].insert(_a);
        rg[a].insert(_c);
        rg[c].insert(_b);
        rg[b].insert(_c);
    }
 
    for (size_t i = 0; (int)i < 2 * n; ++i)
        if (!used[i])
            dfs1(i);
 
    int c = 0;
    while (!order.empty()) {
        if (!comp[order.top()]) {
            dfs2(order.top(), ++c);
        }
        order.pop();
    }
 
    for (size_t i = 0; (int)i < n; ++i) {
        if (comp[i] == comp[n + i]) {
            cout << "NO";
            return 0;
        }
    }
 
    cout << "YES\n";
    for (int i = 0; i < n; ++i) {
        if (comp[i] < comp[n + i]) {
            cout << -(i + 1) << ' ';
        } else {
            cout << i + 1 << ' ';
        }
    }
    return 0;
}