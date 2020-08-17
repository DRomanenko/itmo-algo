#include <iostream>
#include <vector>
 
using namespace std;
 
const size_t MAXS = 2000;
 
vector<vector<pair<size_t, long long>>> g(MAXS);
vector<size_t> isVertexInNegativeCycle;
vector<bool> isVertexAchievableFromNegativeCycle(MAXS, false);
 
void dfs(size_t v) {
    isVertexAchievableFromNegativeCycle[v] = true;
    for (auto to : g[v])
        if (!isVertexAchievableFromNegativeCycle[to.first])
            dfs(to.first);
}
 
struct edge {
    size_t v, to;
    long long weight;
};
 
const long long MAX_NUM = 5000000000000000010LL;
 
vector<edge> edges;
vector<long long> dist(MAXS, MAX_NUM);
 
void fordBellman(size_t n, size_t startV) {
    dist[startV] = 0;
    for (size_t i = 0; i < n - 1; ++i) {
        for (auto curr_edge : edges) {
            if (dist[curr_edge.v] < MAX_NUM) {
                if (dist[curr_edge.to] > dist[curr_edge.v] + curr_edge.weight) {
                    dist[curr_edge.to] = max(dist[curr_edge.v] + curr_edge.weight, -MAX_NUM);
                }
            }
        }
    }
    for (auto curr_edge : edges)
        if (dist[curr_edge.v] < MAX_NUM)
            if (dist[curr_edge.to] > dist[curr_edge.v] + curr_edge.weight)
                isVertexInNegativeCycle.push_back(curr_edge.v);
}
 
int main() {
    /* freopen("truth.in", "r", stdin);
     freopen("truth.out", "w", stdout);*/
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    size_t n, m, s;
    cin >> n >> m >> s;
 
    for (size_t i = 0; i < m; ++i) {
        size_t v, to;
        long long weight;
        cin >> v >> to >> weight;
        g[v - 1].push_back({to - 1, weight});
        edges.push_back({v - 1, to - 1, weight});
    }
 
    fordBellman(n, s - 1);
 
    for (auto v : isVertexInNegativeCycle)
        if (!isVertexAchievableFromNegativeCycle[v])
            dfs(v);
 
    for (size_t v = 0; v < n; ++v)
        if (isVertexAchievableFromNegativeCycle[v])
            cout << "-\n";
        else if (dist[v] == MAX_NUM)
            cout << "*\n";
        else
            cout << dist[v] << '\n';
    return 0;
}