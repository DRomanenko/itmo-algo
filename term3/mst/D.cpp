#include <iostream>
#include <vector>
#include <queue>
#include <set>
 
using namespace std;
 
const size_t MAXS = 3 * 1E5;
 
vector<vector<pair<size_t, size_t>>> g(MAXS);
vector<long long> dist(MAXS, INT32_MAX);
 
void fastDijkstra(size_t startV) {
    dist[startV] = 0;
    set<pair<int, int> > s;
    s.insert({dist[startV], startV});
    while (!s.empty()) {
        int v = s.begin()->second;
        s.erase(s.begin());
 
        for (auto u : g[v]) {
            size_t to = u.first;
            long long currDist = u.second;
            if (dist[to] > dist[v] + currDist) {
                s.erase({dist[v], v});
                dist[to] = dist[v] + currDist;
                s.insert({dist[to], to});
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
        size_t v1, v2, weight;
        cin >> v1 >> v2 >> weight;
        g[v1 - 1].push_back({v2 - 1, weight});
        g[v2 - 1].push_back({v1 - 1, weight});
    }
 
    fastDijkstra(0);
 
    for (size_t i = 0; i < n; ++i)
        cout << dist[i] << ' ';
    return 0;
}