#include <iostream>
#include <vector>
#include <queue>
#include <set>
 
using namespace std;
 
size_t MAXS = 30000;
 
vector<set<size_t>> g(MAXS);
 
queue<int> q;
vector<bool> used(MAXS, false);
vector<unsigned int> dist(MAXS, 0);
 
void bfs(size_t startV) {
    q.push(startV);
    used[startV] = true;
    dist[startV] = 0;
    while (!q.empty()) {
        size_t v = q.front();
        q.pop();
        for (auto to : g[v]) {
            if (!used[to]) {
                q.push(to);
                used[to] = true;
                dist[to] = dist[v] + 1;
            }
        }
    }
}
 
int main() {
    /* freopen("truth.in", "r", stdin);
     freopen("truth.out", "w", stdout);*/
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    size_t n, m;
    cin >> n >> m;
 
    for (size_t i = 0; i < m; ++i) {
        size_t v1, v2;
        cin >> v1 >> v2;
        g[v1 - 1].insert(v2 - 1);
        g[v2 - 1].insert(v1 - 1);
    }
 
    bfs(0);
 
    for (size_t i = 0; i < n; ++i)
        cout << dist[i] << ' ';
    return 0;
}