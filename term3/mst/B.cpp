#include <iostream>
#include <vector>
 
using namespace std;
 
size_t MAXS = 2000;
 
vector<vector<int>> g(MAXS, vector<int>(MAXS));
vector<bool> used(MAXS, false);
vector<long long> dist(MAXS, INT64_MAX);
 
size_t n;
 
void dijkstra(size_t startV, size_t finalV) {
    dist[startV] = 0;
    for (size_t i = 0; i < n; ++i) {
        int indexMinWeight = -1;
        // выбираем самую близкую непомеченную вершину
        for (size_t v = 0; v < n; ++v)
            if (!used[v] && dist[v] < INT64_MAX)
                if (indexMinWeight == -1 || dist[v] < dist[indexMinWeight])
                    indexMinWeight = v;
 
        // нет ближайшей вершины
        if (indexMinWeight == -1 || indexMinWeight == (int)finalV)
            break;
 
        used[indexMinWeight] = true;
 
        for (size_t v = 0; v < n; ++v) {
            int currWeightTo = g[indexMinWeight][v];
 
            if (currWeightTo == -1)
                continue;
 
            if (!used[v])
                dist[v] = min(dist[v], dist[indexMinWeight] + currWeightTo);
        }
    }
}
 
int main() {
    /* freopen("truth.in", "r", stdin);
     freopen("truth.out", "w", stdout);*/
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    size_t s, f;
    cin >> n >> s >> f;
    --s;
    --f;
 
    for (size_t i = 0; i < n; ++i) {
        for (size_t q = 0; q < n; ++q) {
            cin >> g[i][q];
        }
    }
 
    dijkstra(s, f);
 
    /*for (size_t i = 0; i < n; ++i) {
        cout << dist[i] << ' ';
    }*/
 
    if (dist[f] == INT64_MAX)
        cout << -1 << '\n';
    else
        cout << dist[f] << '\n';
    return 0;
}