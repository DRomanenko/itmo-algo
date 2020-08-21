#include <iostream>
#include <vector>
 
using namespace std;
 
size_t MAXS = 1010;
vector<vector<int>> g(MAXS, vector<int>(MAXS, -1)), rg(MAXS, vector<int>(MAXS, -1));
vector<bool> used(MAXS, false);
size_t qty = 0;
int middle = 0;
 
void dfs(vector<vector<int>> &graph, int v) {
    used[v] = true;
    qty++;
    for (size_t to = 0; to < MAXS; ++to) {
        if (graph[v][to] != -1 && !used[to] && graph[v][to] <= middle) {
            dfs(graph, to);
        }
    }
}
 
int main() {
    freopen("avia.in", "r", stdin);
    freopen("avia.out", "w", stdout);
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
 
    size_t n;
    cin >> n;
    for (size_t i = 0; i < n; ++i) {
        for (size_t q = 0; q < n; ++q) {
            cin >> g[i][q];
        }
    }
    for (size_t i = 0; i < n; ++i) {
        for (size_t q = 0; q < n; ++q) {
            rg[i][q] = g[q][i];
        }
    }
    int left = -1, right = 2e9;
    while (right > left + 1) {
        middle = left + ((right - left) >> 1);
        qty = 0;
        used.assign(MAXS, false);
        dfs(g, 0);
        if (qty == n) {
            qty = 0;
            used.assign(MAXS, false);
            dfs(rg, 0);
            if (qty == n)
                right = middle;
            else
                left = middle;
        } else {
            left = middle;
        }
    }
    cout << right << '\n';
    return 0;
}