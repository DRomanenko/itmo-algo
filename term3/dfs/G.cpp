#include <iostream>
#include <vector>
 
using namespace std;
 
const size_t MAXS = 1E5 + 10;
vector<vector<int>> g(MAXS);
vector<int> comps(MAXS, -1);
 
void findComps(int v, int numComp) {
    if (comps[v] > -1) {
        return;
    }
    comps[v] = numComp;
    for (auto to : g[v]) {
        if (comps[to] < 0) {
            findComps(to, numComp);
        }
    }
}
 
int main() {
    freopen("components.in", "r", stdin);
    freopen("components.out", "w", stdout);
 
    size_t n, m;
    cin >> n >> m;
 
    for (size_t i = 0; i < m; ++i) {
        int v1, v2;
        cin >> v1 >> v2;
        g[v1 - 1].push_back(v2 - 1);
        g[v2 - 1].push_back(v1 - 1);
    }
 
    size_t qtyC = 0;
    for (size_t i = 0; i < n; ++i)
        if (comps[i] < 0)
            findComps(i, qtyC++);
 
    cout << qtyC << '\n';
    for (size_t i = 0; i < n; ++i)
        cout << comps[i] + 1 << ' ';
    return 0;
}