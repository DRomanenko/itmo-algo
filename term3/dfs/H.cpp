#include <iostream>
#include <vector>
#include <algorithm>
 
using namespace std;
 
const size_t MAXS = 1E5 + 10;
vector<vector<pair<int, int>>> g(MAXS);
vector<bool> used(MAXS, false);
vector<int> tin(MAXS, 0);
vector<int> d(MAXS, 0);
size_t t = 0;
 
struct bridge {
    int bv;
    int ev;
};
vector<int> bridgesNum;
vector<bridge> bridges;
 
void dfs(int v, int numP) {
    used[v] = true;
    tin[v] = t++;
    d[v] = tin[v];
    for (auto to : g[v]) {
        if (numP == to.second)
            continue;
        if (!used[to.first]) {
            dfs(to.first, to.second);
            d[v] = min(d[v], d[to.first]);
            if (d[to.first] > tin[v]) {
                bridges.push_back({v, to.first});
            }
        } else {
            d[v] = min(d[v], tin[to.first]);
        }
    }
    if (numP != -1 && d[v] == tin[v])
        bridgesNum.push_back(numP);
}
 
int main() {
    freopen("bridges.in", "r", stdin);
    freopen("bridges.out", "w", stdout);
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
 
    size_t n, m;
    cin >> n >> m;
 
    for (size_t i = 0; i < m; ++i) {
        int v1, v2;
        cin >> v1 >> v2;
        g[v1 - 1].push_back({v2 - 1, i});
        g[v2 - 1].push_back({v1 - 1, i});
    }
 
    for (size_t i = 0; i < n; ++i)
        if (!used[i])
            dfs(i, -1);
 
    cout << bridges.size() << '\n';
    sort(bridgesNum.begin(), bridgesNum.end());
    for (auto num : bridgesNum)
        cout << num + 1 << ' ';
    return 0;
}