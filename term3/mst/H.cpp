#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
 
using namespace std;
 
const size_t MAXS = 20000;
 
struct edge {
    size_t v, to;
    long long weight;
 
    bool operator<(const edge &other) const { return this->weight < other.weight; }
};
 
vector<edge> edges;
vector<size_t> parent(MAXS), rang(MAXS);
 
void make_v(size_t v) {
    parent[v] = v;
    rang[v] = 0;
}
 
size_t getRoot(size_t v) {
    if (parent[v] == v)
        return v;
    return parent[v] = getRoot(parent[v]);
}
 
void join(size_t v, size_t to) {
    size_t root_v = getRoot(v), root_to = getRoot(to);
 
    if (root_v != root_to) {
        if (rang[root_v] < rang[root_to])
            swap(root_v, root_to);
        parent[root_to] = root_v;
        if (rang[root_v] == rang[root_to])
            ++rang[root_v];
    }
}
 
long long kruskal() {
    sort(edges.begin(), edges.end());
    long long ans = 0;
    for (auto curr_edge : edges) {
        if (getRoot(curr_edge.v) != getRoot(curr_edge.to)) {
            join(curr_edge.v, curr_edge.to);
            ans += curr_edge.weight;
        }
    }
    return ans;
}
 
int main() {
    /* freopen("truth.in", "rank", stdin);
    freopen("truth.out", "w", stdout); */
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    size_t n, m;
    cin >> n >> m;
 
    for (size_t v = 0; v < n; ++v)
        make_v(v);
 
    for (size_t i = 0; i < m; ++i) {
        size_t v, to;
        long long weight;
        cin >> v >> to >> weight;
        edges.push_back({v - 1, to - 1, weight});
    }
 
    cout << kruskal() << '\n';
    return 0;
}