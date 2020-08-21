#include <iostream>
#include <vector>
 
using namespace std;
 
const size_t MAXS = 1E5;
vector<vector<int>> g(MAXS);
vector<bool> used(MAXS, false);
vector<bool> d1(MAXS, false), d2(MAXS, false);
 
void dfs(int v) {
    used[v] = true;
    for (auto to : g[v]) {
        if (!used[to]) {
            dfs(to);
        }
        // Начиная с этой вершины игрок №1 может выиграть
        d1[v] = d1[v] || !d2[to];
        // Начиная с этой вершины игрок №2 может выиграть
        d2[v] = d2[v] || !d1[to];
    }
}
 
int main() {
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
 
    size_t n, m;
    int s;
    cin >> n >> m >> s;
    s--;
    for (size_t i = 0; i < m; ++i) {
        int v1, v2;
        cin >> v1 >> v2;
        g[v1 - 1].push_back(v2 - 1);
    }
 
    dfs(s);
 
    if (d1[s]) {
        cout << "First player wins";
    } else {
        cout << "Second player wins";
    }
    return 0;
}