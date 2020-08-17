#include <iostream>
#include <vector>
 
using namespace std;
 
struct edge {
    int v;
    int w;
};
 
const size_t MAXS = 1E5;
vector<vector<edge>> g(MAXS);
vector<bool> used(MAXS, false);
vector<int> order;
 
void topSort(int v) {
    used[v] = true;
    for (auto to : g[v]) {
        if (!used[to.v]) {
            topSort(to.v);
        }
    }
    order.push_back(v);
}
 
int main() {
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
 
    order.reserve(MAXS);
    order.shrink_to_fit();
 
    size_t n, m, s, t;
    cin >> n >> m >> s >> t;
    s--;
    t--;
    for (size_t i = 0; i < m; ++i) {
        int v1, v2, w;
        cin >> v1 >> v2 >> w;
        g[v1 - 1].push_back({v2 - 1, w});
    }
 
    topSort(s);
    if (!used[t]) {
        cout << "Unreachable";
        return 0;
    }
 
    vector<long long> d(MAXS, LONG_LONG_MAX);
    d[s] = 0;
 
    for (int i = order.size() - 1; i >= 0; --i) {
        if (!g[order[i]].empty()) {
            for (auto to : g[order[i]]) {
                d[to.v] = min(d[to.v], d[order[i]] + to.w);
            }
        }
    }
    cout << d[t];
    return 0;
}