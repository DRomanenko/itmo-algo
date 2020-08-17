#include <iostream>
#include <vector>
 
using namespace std;
 
enum color {
    WHITE, GRAY, BLACK
};
 
const size_t MAXS = 1E5 + 10;
vector<vector<int>> g(MAXS);
vector<int> color(MAXS, WHITE);
 
void dfs(int v, char currColor) {
    color[v] = currColor;
 
    for (auto u : g[v]) {
        if (color[u] == WHITE) {
            dfs(u, currColor == GRAY ? BLACK : GRAY);
        } else if (color[u] == currColor) {
            cout << "NO\n";
            exit(0);
        }
    }
}
 
int main() {
    freopen("bipartite.in", "r", stdin);
    freopen("bipartite.out", "w", stdout);
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
        if (!color[i])
            dfs(i, GRAY);
 
    cout << "YES\n";
    return 0;
}