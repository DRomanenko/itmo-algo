#include <iostream>
#include <vector>
#include <stack>
#include <set>
 
using namespace std;
 
size_t MAXS = 2000;
 
vector<set<pair<int, int>>> g(MAXS), rg(MAXS);
vector<bool> used(MAXS, false);
vector<int> comp(MAXS, -1);
stack<size_t> order;
 
int middle;
bool flag;
 
void dfs1(size_t v) {
    used[v] = true;
    for (auto to : g[v])
        if (!used[to.first] && ((int)to.second <= middle))
            dfs1(to.first);
    order.push(v);
}
 
void dfs2(size_t v, int c) {
    comp[v] = c;
    for (auto to : rg[v])
        if (comp[to.first] == -1 && ((int)to.second <= middle))
            dfs2(to.first, c);
}
 
int main() {
    freopen("truth.in", "r", stdin);
    freopen("truth.out", "w", stdout);
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    size_t n, m;
    cin >> n >> m;
 
    for (size_t i = 0; i < m; ++i) {
        size_t v1, v2;
        char statement;
        cin >> v1 >> v2 >> statement;
        --v1;
        --v2;
        if (statement == 'T') {
            g[v1].insert({v2, i});
            g[v2].insert({v1, i});
            g[v1 + n].insert({v2 + n, i});
            g[v2 + n].insert({v1 + n, i});
            rg[v1].insert({v2, i});
            rg[v2].insert({v1, i});
            rg[v1 + n].insert({v2 + n, i});
            rg[v2 + n].insert({v1 + n, i});
        }
        if (statement == 'L') {
            g[v1 + n].insert({v2, i});
            g[v2 + n].insert({v1, i});
            g[v1].insert({v2 + n, i});
            g[v2].insert({v1 + n, i});
            rg[v1 + n].insert({v2, i});
            rg[v2 + n].insert({v1, i});
            rg[v1].insert({v2 + n, i});
            rg[v2].insert({v1 + n, i});
        }
    }
 
    int left = -1, right = m;
    while (right > left + 1) {
        middle = left + ((right - left) >> 1);
 
        flag = true;
        used.assign(MAXS, false);
        while (!order.empty())
            order.pop();
 
        for (size_t i = 0; i < 2 * n; ++i)
            if (!used[i])
                dfs1(i);
 
        comp.assign(MAXS, -1);
        used.assign(MAXS, false);
 
        int c = 0;
        while (!order.empty()) {
            if (comp[order.top()] == -1) {
                dfs2(order.top(), ++c);
            }
            order.pop();
        }
 
        for (size_t i = 0; i < n; ++i) {
            if (comp[i] == comp[n + i]) {
                flag = false;
                break;
            }
        }
 
        if (flag) {
            left = middle;
        } else {
            right = middle;
        }
    }
    cout << right << '\n';
    return 0;
}