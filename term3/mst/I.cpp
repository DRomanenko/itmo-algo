#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

const size_t MAXS = 1000;

struct edge {
    size_t v, to;
    long long weight;

    bool operator<(const edge &other) const { return this->weight < other.weight; }
};

vector<edge> edges;
vector<size_t> parent(MAXS, 0), rang(MAXS, 0);

void make_v(size_t v) {
    parent[v] = v;
    rang[v] = 0;
}

void resizeSet(size_t sizeG) {
    rang.assign(sizeG, 0);
    parent.assign(sizeG, 0);
    for (size_t v = 0; v < sizeG; ++v)
        make_v(v);
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

bool kruskal(size_t sizeG, size_t start, long long &minWeight, long long &maxWeight) {
    size_t qtyOfVisitedV = 1;
    for (size_t i = start; i < edges.size(); ++i) {
        auto curr_edge = edges[i];
        if (getRoot(curr_edge.v) != getRoot(curr_edge.to)) {
            maxWeight = max(maxWeight, curr_edge.weight);
            minWeight = min(minWeight, curr_edge.weight);
            join(curr_edge.v, curr_edge.to);
            (++qtyOfVisitedV);
        }
    }
    return (qtyOfVisitedV == sizeG);
}

int main() {
    /* freopen("truth.in", "r", stdin);
    freopen("truth.out", "w", stdout); */
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    size_t n, m;
    cin >> n >> m;

    for (size_t i = 0; i < m; ++i) {
        size_t v, to;
        long long weight;
        cin >> v >> to >> weight;
        edges.push_back({v - 1, to - 1, weight});
    }   

    sort(edges.begin(), edges.end());
    
    long long minDiff = INT64_MAX;
    for (size_t i = 0; i < m; ++i) {
        resizeSet(n);
        long long minWeight = INT64_MAX, maxWeight = INT64_MIN;
        if (kruskal(n, i, minWeight, maxWeight))
            minDiff = min(minDiff, maxWeight - minWeight);
    }

    if (minDiff != INT64_MAX)
        cout << "YES\n" << minDiff << '\n';
    else
        cout << "NO\n";
    return 0;
}

