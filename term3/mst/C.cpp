#include <iostream>
#include <vector>
 
using namespace std;
 
size_t MAXS = 200;
 
vector<vector<long long>> dist(MAXS, vector<long long>(MAXS, INT32_MAX));
 
void floydWarshall(size_t n) {
    for (size_t i = 0; i < n; ++i)
        dist[i][i] = 0;
 
    for (size_t i = 0; i < n; ++i)
        for (size_t v = 0; v < n; ++v)
            for (size_t to = 0; to < n; ++to)
                dist[v][to] = min(dist[v][to], dist[v][i] + dist[i][to]);
}
 
void printDistMatrix(size_t n) {
    for (size_t v = 0; v < n; ++v) {
        for (size_t to = 0; to < n; ++to) {
            cout << dist[v][to] << ' ';
        }
        cout << '\n';
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
        dist[v1 - 1][v2 - 1] = weight;
    }
 
    floydWarshall(n);
    printDistMatrix(n);
    return 0;
}