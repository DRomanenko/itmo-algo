#include <iostream>
#include <vector>

using namespace std;

vector<vector<long long>> graph;
vector<long long> depth, weight, parent, heavy;

void dfs (long long v) {
    weight[v] = 1;
    depth[v] = (parent[v] != -1) ? depth[parent[v]] + 1 : 0;
    for (auto u : graph[v]) {
        dfs(u);
        weight[v] += weight[u];
        if (heavy[v] == -1 || (weight[heavy[v]] < weight[u])) {
            heavy[v] = u;
        }
    }
}

long long count = 1, num = 0;
vector<long long> order, number, chain;

void HLD (long long v) {
    order[count] = v;
    if (number[num] == -1) {
        number[num] = count;
    }
    ++count;
    chain[v] = num;
    if (heavy[v] != -1) {
        HLD(heavy[v]);
    }
    for (auto u : graph[v]) {
        if (u != heavy[v]) {
            ++num;
            HLD(u);
        }
    }
}

long long max (long long a, long long b) {
    return (a > b) ? a : b;
}

vector<long long> group, max_depth;

long long calc_ans (long long v, long long num_group) {
    long long ans = 0;
    while (true) {
        long long curr = order[number[chain[v]]];
        if (group[curr] < num_group) {
            max_depth[curr] = depth[curr];
        }
        ans += max(0, depth[v] - max_depth[curr] + 1);
        max_depth[curr] = max(max_depth[curr], depth[v] + 1);
        group[curr] = num_group;
        if (parent[curr] != -1) {
            v = parent[curr];
        } else {
            return ans;
        }
    }
}

int main () {
    iostream::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    long long n;
    cin >> n;
    parent.resize(n + 1), depth.assign(n + 1, 0), weight.assign(n + 1, 0), heavy.assign(n + 1, -1);
    graph.resize(n + 1, vector<long long>(0));
    long long root = 0;
    for (int i = 1; i <= n; ++i) {
        cin >> parent[i];
        if (parent[i] == -1) {
            root = i;
            continue;
        }
        graph[parent[i]].push_back(i);
    }

    dfs(root);
    order.resize(n + 1, 0), number.assign(n + 1, -1), chain.resize(n + 1, -1);
    HLD(root);

    long long g;
    cin >> g;
    group.assign(n + 1, -1), max_depth.assign(n + 1, -1);
    for (int i = 0; i < g; ++i) {
        long long k, v, ans = 0;
        cin >> k;
        for (int q = 1; q <= k; ++q) {
            cin >> v;
            ans += calc_ans(v, i);
        }
        cout << ans << endl;
    }
    return 0;
}