#include <iostream>
#include <vector>
#include <algorithm>
 
using namespace std;
 
const long long INF = 1e18 * 9;
 
struct edge {
    size_t from, to;
    long long capacity, flow, cost;
};
 
size_t trueSize, source, sink;
vector<long long> p, dist;
vector<int> ptr, lasts, edgeToUpdate;
vector<bool> used;
vector<edge> edges;
 
void addEdge(size_t from, size_t to, long long capacity, long long cost) {
    edges.push_back({from, to, capacity, 0, cost});
    edges.push_back({to, from, 0, 0, -cost});
    ptr.push_back(lasts[from]);
    ptr.push_back(lasts[to]);
    lasts[from] = edges.size() - 2;
    lasts[to] = edges.size() - 1;
}
 
void pushFlow(int num, long long flow) {
    edges[num].flow += flow;
    edges[num ^ 1].flow -= flow;
}
 
bool isINF(long long t) {
    return t >= INF;
}
 
long long w(int e) {
    long long ans = edges[e].cost + p[edges[e].from] - p[edges[e].to];
    return ans;
}
 
void FordBellman() {
    p.resize(trueSize, INF);
    p[source] = 0;
    while (true) {
        bool change = false;
        for (const edge &t : edges) {
            if (t.flow == t.capacity)
                continue;
            if (isINF(p[t.from]))
                continue;
            if (p[t.to] > p[t.from] + t.cost) {
                p[t.to] = p[t.from] + t.cost;
                change = true;
            }
        }
        if (!change)
            break;
    }
}
 
bool Dijkstra() {  /// working with dist
    dist.assign(trueSize, INF);
    used.assign(trueSize, false);
    dist[source] = 0;
 
    for (size_t i = 0; i < trueSize; ++i) {
        int cur = -1;
        for (size_t j = 0; j < trueSize; ++j)
            if (!used[j] && !isINF(dist[j]))
                if (cur == -1 || dist[j] < dist[cur])
                    cur = j;
        if (cur == -1)
            break;
        used[cur] = true;
 
        for (int j = lasts[cur]; j != -1; j = ptr[j]) {
            if (edges[j].flow == edges[j].capacity)
                continue;
            int to = edges[j].to;
            if (isINF(dist[to]) || (!isINF(dist[to]) && dist[cur] + w(j) < dist[to])) {
                dist[to] = dist[cur] + w(j);
                edgeToUpdate[to] = j;
            }
        }
    }
    return !isINF(dist[sink]);
}
 
pair<long long, long long> MinCostMaxFlowAlgorithm() {
    FordBellman();
    p.resize(trueSize);
    edgeToUpdate.resize(trueSize);
    long long maxFlow = 0, cost = 0;
    while (Dijkstra()) {  /// exist path to sink in layered net
        /// find the shortest path to sink in layered net
        long long flow = INF;
        size_t cur = sink;
        while (cur != source) {
            int j = edgeToUpdate[cur];
            flow = min(flow, edges[j].capacity - edges[j].flow);
            cur = edges[j].from;
        }
        /// add flow on this path
        cur = sink;
        while (cur != source) {
            int j = edgeToUpdate[cur];
            pushFlow(j, flow);
            cur = edges[j].from;
            cost += flow * edges[j].cost;
        }
        maxFlow += flow;
        /// for(v) p[v] += dist[v]
        for (size_t i = 0; i < trueSize; ++i) {
            if (isINF(dist[i]))
                p[i] = INF;
            else
                p[i] += dist[i];
        }
    }
    return {maxFlow, cost};
}
 
// const string FILE_NAME = "chvatal";
int main() {
    // freopen((FILE_NAME + ".in").c_str(), "r", stdin);
    // freopen((FILE_NAME + ".out").c_str(), "w", stdout);
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    size_t n, m;
    cin >> n >> m;
 
    trueSize = n;
    source = 0;
    sink = trueSize - 1;
    lasts.resize(trueSize, -1);
 
    for (size_t i = 0; i < m; ++i) {
        size_t a, b;
        long long c, price;
        cin >> a >> b >> c >> price;
        addEdge(a - 1, b - 1, c, price);
    }
 
    cout << MinCostMaxFlowAlgorithm().second << '\n';
    return 0;
}