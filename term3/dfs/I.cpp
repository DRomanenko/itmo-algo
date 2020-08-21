#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
 
using namespace std;
 
const size_t MAXS = 1E5 + 10;
set<int> dots;
vector<vector<int>> g(MAXS);
vector<bool> used(MAXS, false);
vector<int> tin(MAXS, INT32_MAX);
vector<int> d(MAXS, INT32_MAX);
size_t t = 0;
 
void dfs(int v, int p) {
    used[v] = true;
    tin[v] = t++;
    d[v] = tin[v];
    int qtyChild = 0;
    for (auto to : g[v]) {
        if (p == to)
            continue;
        if (used[to]) {
            d[v] = min(d[v], tin[to]);
        } else {
            dfs(to, v);
            ++qtyChild;
            d[v] = min(d[v], d[to]);
            if (p != -1 && d[to] >= tin[v])
                dots.insert(v);
        }
    }
    if (p == -1 && qtyChild > 1)
        dots.insert(v);
}
 
int main() {
    freopen("points.in", "r", stdin);
    freopen("points.out", "w", stdout);
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
 
    size_t n, m;
    cin >> n >> m;
 
    for (size_t i = 0; i < m; ++i) {
        int v1, v2;
        cin >> v1 >> v2;
        g[v1 - 1].push_back(v2 - 1);
        g[v2 - 1].push_back(v1 - 1);
    }
 
    for (size_t i = 0; i < n; ++i)
        if (!used[i])
            dfs(i, -1);
 
    cout << dots.size() << '\n';
    for (auto v : dots)
        cout << v + 1 << '\n';
    return 0;
}