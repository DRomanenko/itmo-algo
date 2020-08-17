#include <iostream>
#include <vector>

using namespace std;

vector<vector<int>> graph;
vector<bool> is_visit, is_deleted;
vector<int> weight, parent;

void dfs (int v) {
    weight[v] = 1;
    is_visit[v] = true;
    for (auto u : graph[v]) {
        if (!is_deleted[u] && !is_visit[u]) {
            dfs(u);
            weight[v] += weight[u];
        }
    }
}

int find_centroid (int v, int size) {
    is_visit[v] = true;
    for (auto child : graph[v]) {
        if (!is_visit[child]) {
            if (!is_deleted[child] && weight[child] > size / 2) {
                return find_centroid(child, size);
            }
        }
    }
    return v;
}

int decompose (int v) {
    if (weight[v] == 1) {
        is_deleted[v] = true;
        return v;
    }
    is_visit.assign(is_visit.size(), false);
    dfs(v);
    if (weight[v] == 1) {
        is_deleted[v] = true;
        return v;
    }
    is_visit.assign(is_visit.size(), false);
    int centroid = find_centroid(v, weight[v]);
    is_deleted[centroid] = true;
    for (auto u : graph[centroid]) {
        if (!is_deleted[u]) {
            parent[decompose(u)] = centroid;
        }
    }
    return centroid;
}

int main () {
    iostream::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int n;
    cin >> n;
    is_visit.resize(n + 1), is_deleted.resize(n + 1);
    weight.resize(n + 1), parent.resize(n + 1);
    graph.resize(n + 1, vector<int>(0));
    for (int i = 1; i < n; i++) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }
    (void) decompose(1);
    for (int i = 1; i < n + 1; i++) {
        cout << parent[i] << " ";
    }
    return 0;
}