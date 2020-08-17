#include <iostream>
#include <vector>
#include <stack>
 
using namespace std;
 
const size_t MAXS = 500;
 
struct edge {
    size_t v, to;
    long long weight;
};
 
vector<edge> edges;
vector<int> parent(MAXS);
vector<long long> dist(MAXS, INT32_MAX);
 
int fordBellman(size_t n, size_t startV) {
    dist[startV] = 0;
    for (size_t i = 0; i < n; ++i) {
        for (auto curr_edge : edges) {
            if (dist[curr_edge.v] < INT32_MAX) {
                if (dist[curr_edge.to] > dist[curr_edge.v] + curr_edge.weight) {
                    dist[curr_edge.to] = max(dist[curr_edge.v] + curr_edge.weight,
                                             (long long) (INT64_MIN + 1));
                    parent[curr_edge.to] = curr_edge.v;
                }
            }
        }
    }
    int save = -1;
    for (auto curr_edge : edges) {
        if (dist[curr_edge.v] < INT32_MAX) {
            if (dist[curr_edge.to] > dist[curr_edge.v] + curr_edge.weight) {
                parent[curr_edge.to] = curr_edge.v;
                save = curr_edge.to;
                break;
            }
        }
    }
    return save;
}
 
int main() {
    /* freopen("truth.in", "r", stdin);
     freopen("truth.out", "w", stdout);*/
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    size_t n;
    cin >> n;
 
    for (size_t v = 0; v < n; ++v) {
        for (size_t to = 0; to < n; ++to) {
            long long weight;
            cin >> weight;
            if (weight != (long long) 1e9)
                edges.push_back({v, to, weight});
        }
    }
 
    // Добавляем фиктивную вершину, из которой есть фиктивное ребро веса 0 во все остальные
    for (size_t v = 0; v < n; ++v)
        edges.push_back({n, v, 0});
 
    int eCycle, bCycle = eCycle = fordBellman(n, n);
    if (eCycle != -1) {
        cout << "YES\n";
 
        for (size_t i = 0; i < n; ++i) {
            bCycle = parent[bCycle];
        }
 
        stack<int> s;
 
        for (int i = parent[bCycle]; i != bCycle; i = parent[i]) {
            s.push(i);
        }
        s.push(bCycle);
        s.push(parent[bCycle]);
 
        cout << s.size() << '\n';
        while (!s.empty()) {
            cout << s.top() + 1 << ' ';
            s.pop();
        }
    } else {
        cout << "NO\n";
    }
    return 0;
}