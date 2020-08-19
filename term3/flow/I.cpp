#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
 
using namespace std;
 
const long long INF = 1e18 * 9;
 
struct edge {
    size_t from, to;
    long long capacity, flow, cost;
};
 
size_t trueSize, source, sink;
std::vector<long long> p, dist;
std::vector<int> ptr, lasts, edgeToUpdate;
std::vector<bool> used;
std::vector<edge> edges;
 
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
        for (size_t q = 0; q < trueSize; ++q)
            if (!used[q] && !isINF(dist[q]))
                if (cur == -1 || dist[q] < dist[cur])
                    cur = q;
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
            flow = std::min(flow, edges[j].capacity - edges[j].flow);
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
 
size_t n_, s_, t_, size_;
std::vector<int> d_, ptr_, lasts_, edgesPtr_;
std::vector<bool> used_;
std::vector<edge> edges_;
 
void addEdge_(size_t from, size_t to, long long capacity) {
    edges_.push_back({from, to, capacity, 0, 0});
    edges_.push_back({to, from, 0, 0, 0});
    ptr_.push_back(lasts_[from]);
    ptr_.push_back(lasts_[to]);
    lasts_[from] = edges_.size() - 2;
    lasts_[to] = edges_.size() - 1;
}
 
void pushFlow_(int num, long long flow) {
    edges_[num].flow += flow;
    edges_[num ^ 1].flow -= flow;
}
 
bool bfs() {
    d_.assign(size_, -1);
    d_[s_] = 0;
    std::queue<int> q;
    q.push(s_);
    while (!q.empty()) {
        int temp = q.front();
        for (int i = lasts_[temp]; i != -1; i = ptr_[i])
            if (d_[edges_[i].to] == -1 && edges_[i].capacity - edges_[i].flow) {
                d_[edges_[i].to] = d_[temp] + 1;
                q.push(edges_[i].to);
            }
        q.pop();
    }
    return d_[t_] != -1;
}
 
long long dfs(size_t v, long long flow) {
    if (!flow)
        return 0;
    if (v == t_)
        return flow;
    for (int i = edgesPtr_[v]; i != -1; i = ptr_[i], edgesPtr_[v] = i)
        if ((d_[edges_[i].to] == d_[v] + 1) && edges_[i].capacity - edges_[i].flow) {
            if (long long pushed =
                    dfs(edges_[i].to,
                        min(flow, edges_[i].capacity - edges_[i].flow))) {
                pushFlow_(i, pushed);
                return pushed;
            }
        }
    return 0;
}
 
long long DinicMaxFlowAlgorithm() {
    long long flow = 0;
    while (bfs()) {
        edgesPtr_ = lasts_;
        while (true) {
            used_.assign(size_, false);
            long long pushed = dfs(s_, INF);
            if (!pushed)
                break;
            flow += pushed;
        }
    }
    return flow;
}
 
// const string FILE_NAME = "chvatal";
int main() {
    // freopen((FILE_NAME + ".in").c_str(), "r", stdin);
    // freopen((FILE_NAME + ".out").c_str(), "w", stdout);
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    size_t n, k;
    cin >> n >> k;
 
    trueSize = 2 * n + 3;
    source = 0;
    sink = trueSize - 2;
    lasts.resize(trueSize, -1);
 
    for (size_t i = 0; i < n; ++i) {
        for (size_t q = 0; q < n; ++q) {
            long long cost;
            cin >> cost;
            addEdge(i + 1, n + 1 + q, 1, cost);
        }
    }
    for (size_t i = 0; i < n; ++i) {
        addEdge(trueSize - 1, i + 1, k, 0);
        addEdge(n + 1 + i, trueSize - 2, k, 0);
    }
    addEdge(0, trueSize - 1, k * n, 0);
 
    cout << MinCostMaxFlowAlgorithm().second << '\n';
    n_ = trueSize, s_ = source, t_ = sink, size_ = trueSize;
    lasts_.resize(n_, -1);
 
    for (size_t i = 0; i < n; ++i) {
        addEdge_(n_ - 1, i + 1, 1);
        addEdge_(n + 1 + i, n_ - 2, 1);
    }
    addEdge_(0, trueSize - 1, n * k);
 
    for (size_t i = 1; i <= n; ++i)
        for (int j = lasts[i]; j != -1; j = ptr[j])
            if (edges[j].flow == 1)
                addEdge_(i, edges[j].to, 1);
 
    for (size_t i = 0; i < k; ++i) {
        DinicMaxFlowAlgorithm();
        for (size_t q = 1; q <= n; ++q)
            for (int t = lasts_[q]; t != -1; t = ptr_[t])
                if (edges_[t].flow == 1) {
                    cout << edges_[t].to - n << " ";
                    edges_[t].flow = edges_[t].capacity =
                    edges_[t ^ 1].flow =
                    edges_[t ^ 1].capacity = 0;
                    break;
                }
 
        cout << '\n';
        for (size_t q = n + 1; q <= 2 * n; ++q)
            for (int t = lasts_[q]; t != -1; t = ptr_[t])
                if (edges_[t].to == trueSize - 2)
                    pushFlow_(t, -1);
        for (int t = lasts_[trueSize - 1]; t != -1; t = ptr_[t])
            if (edges_[t].to >= 1 && edges_[t].to <= n)
                pushFlow_(t, -1);
    }
    return 0;
}