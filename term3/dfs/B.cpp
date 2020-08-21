#include <iostream>
#include <vector>
 
using namespace std;
 
const size_t MAXS = 1E5;
vector<vector<int>> g(MAXS);
vector<vector<int>> rg(MAXS);
vector<bool> used(MAXS, false);
vector<int> comps(MAXS, -1);
vector<int> order;
 
void topSort(int v) {
    used[v] = true;
    for (auto to : g[v]) {
        if (!used[to]) {
            topSort(to);
        }
    }
    order.push_back(v);
}
 
void findComps(int v, int numComp) {
    if (comps[v] > -1) {
        return;
    }
    comps[v] = numComp;
    for (auto to : rg[v]) {
        if (comps[to] < 0) {
            findComps(to, numComp);
        }
    }
}
 
int main() {
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
 
    order.reserve(MAXS);
 
    size_t n, m;
    cin >> n >> m;
 
    for (size_t i = 0; i < m; ++i) {
        int v1, v2;
        cin >> v1 >> v2;
        g[v1 - 1].push_back(v2 - 1);
        rg[v2 - 1].push_back(v1 - 1);
    }
    for (size_t i = 0; i < n; ++i)
        if (!used[i])
            topSort(i);
 
    size_t qtyC = 0;
    for (int i = n - 1; i >= 0; --i)
        if (comps[order[i]] < 0)
            findComps(order[i], qtyC++);
 
    cout << qtyC << '\n';
    for (size_t i = 0; i < n; ++i)
        cout << comps[i] + 1 << ' ';
 
    return 0;
}