#include <iostream>
#include <vector>

using namespace std;

vector<vector<long long>> graph;
vector<long long> weight;
vector<bool> is_deleted;

void dfs1 (long long v, int parent) {
    weight[v] = 1;
    for (auto u : graph[v]) {
        if (!is_deleted[u] && parent != u) {
            dfs1(u, v);
            weight[v] += weight[u];
        }
    }
}

int find_centroid (long long v, long long size, long long parent) {
    for (auto u : graph[v]) {
        if (parent != u && !is_deleted[u] && weight[u] > size / 2) {
            return find_centroid(u, size, v);
        }
    }
    return v;
}

struct WhiteBlackCounter {
    long long white_count, black_count;

    WhiteBlackCounter () :
            white_count(0LL), black_count(0LL) {
    }

    WhiteBlackCounter (long long new_white_count, long long new_black_count) :
            white_count(new_white_count), black_count(new_black_count) {
    }

    ~WhiteBlackCounter () = default;
};

vector<vector<WhiteBlackCounter>> dist;
vector<vector<vector<WhiteBlackCounter>>> all_dist;
/*** first -> pair(num_centroids) second -> dist ***/
vector<vector<pair<pair<long long, long long>, long long>>> centroid_parent;

void upd_size (long long v_1, long long v_2, long long depth) {
    if (dist[v_1].size() <= depth) { dist[v_1].push_back(WhiteBlackCounter()); }
    ++dist[v_1][depth].black_count;
    if (all_dist[v_1][v_2].size() <= depth) { all_dist[v_1][v_2].push_back(WhiteBlackCounter()); }
    ++all_dist[v_1][v_2][depth].black_count;
}

void dfs2 (long long v_1, long long v_2, long long v, long long depth, long long parent) {
    upd_size(v_1, v_2, depth);
    for (auto u : graph[v]) {
        if (!is_deleted[u] && parent != u) {
            dfs2(v_1, v_2, u, depth + 1, v);
            centroid_parent[u].push_back({{v_1, v_2}, depth + 1});
        }
    }
}

/*** first -> sum second -> mul ***/
vector<pair<WhiteBlackCounter, WhiteBlackCounter>> ans_dist;
vector<vector<pair<WhiteBlackCounter, WhiteBlackCounter>>> ans_all_dist;

void upd (long long v) {
    dist[v].push_back({0, 1});
    is_deleted[v] = true;
    all_dist[v].resize(graph[v].size());
    ans_all_dist[v].resize(graph[v].size());
}

long long n;

void calc_all (long long v, long long num) {
    for (int q = 0; q < all_dist[v][num].size(); ++q) {
        ans_all_dist[v][num].first.black_count += all_dist[v][num][q].black_count;
        ans_all_dist[v][num].second.black_count += all_dist[v][num][q].black_count * q;
    }
    all_dist[v].resize(0);
}

void calc_once (long long v) {
    for (int q = 0; q < dist[v].size(); ++q) {
        ans_dist[v].first.black_count += dist[v][q].black_count;
        ans_dist[v].second.black_count += dist[v][q].black_count * q;
    }
    dist[v].resize(0);
    all_dist[v].resize(0);
}

void decompose (long long v) {
    dfs1(v, -1);
    v = find_centroid(v, weight[v], -1);
    upd(v);
    for (int i = 0; i < graph[v].size(); ++i) {
        if (!is_deleted[graph[v][i]]) {
            centroid_parent[graph[v][i]].push_back({{v, i}, 1});
            all_dist[v][i].push_back({0, 0});
            dfs2(v, i, graph[v][i], 1, -1);
            calc_all(v, i);
            decompose(graph[v][i]);
        }
    }
    calc_once(v);
}

vector<bool> color;

void initialization (int size) {
    weight.resize(size), is_deleted.resize(size, false);

    dist.resize(size);
    all_dist.resize(size);
    centroid_parent.resize(size);

    ans_dist.resize(size);
    ans_all_dist.resize(size);

    color.resize(size, false);
}

void change_color (long long v) {
    WhiteBlackCounter ch = color[v] ? WhiteBlackCounter{-1, 1} : WhiteBlackCounter{1, -1};
    color[v] = color[v] ^ true;
    ans_dist[v].first.white_count += ch.white_count;
    ans_dist[v].first.black_count += ch.black_count;
    for (auto u : centroid_parent[v]) {
        ans_all_dist[u.first.first][u.first.second].first.white_count += ch.white_count;
        ans_all_dist[u.first.first][u.first.second].first.black_count += ch.black_count;
        ans_dist[u.first.first].first.white_count += ch.white_count;
        ans_dist[u.first.first].first.black_count += ch.black_count;
        ans_all_dist[u.first.first][u.first.second].second.white_count += ch.white_count * u.second;
        ans_all_dist[u.first.first][u.first.second].second.black_count += ch.black_count * u.second;
        ans_dist[u.first.first].second.white_count += ch.white_count * u.second;
        ans_dist[u.first.first].second.black_count += ch.black_count * u.second;
    }
}

long long find_sum_dist (long long v) {
    WhiteBlackCounter res = ans_dist[v].second;
    for (auto u : centroid_parent[v]) {
        res.white_count +=
                (ans_dist[u.first.first].first.white_count -
                 ans_all_dist[u.first.first][u.first.second].first.white_count) * u.second
                +
                (ans_dist[u.first.first].second.white_count -
                 ans_all_dist[u.first.first][u.first.second].second.white_count);
        res.black_count +=
                (ans_dist[u.first.first].first.black_count -
                 ans_all_dist[u.first.first][u.first.second].first.black_count) * u.second
                +
                (ans_dist[u.first.first].second.black_count -
                 ans_all_dist[u.first.first][u.first.second].second.black_count);
    }
    return color[v] ? res.white_count : res.black_count;
}

int main () {
    int m;
    cin >> n >> m;
    graph.resize(n);
    for (int i = 0; i < n - 1; ++i) {
        int v, u;
        cin >> v >> u;
        graph[--v].push_back(--u);
        graph[u].push_back(v);
    }
    initialization(n);
    decompose(0LL);
    dist.clear();
    all_dist.clear();
    for (int i = 0; i < m; ++i) {
        int command, v;
        cin >> command >> v;
        switch (command) {
            case 1:
                change_color(--v);
                break;
            case 2:
                cout << find_sum_dist(--v) << '\n';
                break;
            default:
                exit(-1);
        }
    }
    return 0;
}