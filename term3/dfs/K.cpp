#include <iostream>
#include <vector>
#include <stack>
#include <map>
 
using namespace std;
 
const size_t MAXS = 1E5 + 10;
vector<vector<pair<int, int>>> g(MAXS);
vector<bool> used(MAXS, false);
vector<int> tin(MAXS, 0);
vector<int> d(MAXS, 0);
size_t t = 0;
 
struct bridge {
    int bv;
    int ev;
};
vector<int> bridgesNum;
vector<bridge> bridges;
 
stack<int> s;
int maxColor = 0;
vector<int> numComps(MAXS, 0);
 
void findComps(int v) {
    ++maxColor;
    int last = -1;
    while (last != v && !s.empty()) {
        last = s.top();
        numComps[last] = maxColor;
        s.pop();
    }
}
 
void dfs(int v, int numP) {
    used[v] = true;
    tin[v] = t++;
    d[v] = tin[v];
    s.push(v);
    for (auto to : g[v]) {
        if (numP == to.second)
            continue;
        if (!used[to.first]) {
            dfs(to.first, to.second);
            d[v] = min(d[v], d[to.first]);
            if (d[to.first] > tin[v]) {
                findComps(to.first);
            }
        } else {
            d[v] = min(d[v], tin[to.first]);
        }
    }
}
 
int main() {
    freopen("bicone.in", "r", stdin);
    freopen("bicone.out", "w", stdout);
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
 
    size_t n, m;
    cin >> n >> m;
 
    for (size_t i = 0; i < m; ++i) {
        int v1, v2;
        cin >> v1 >> v2;
        g[v1 - 1].push_back({v2 - 1, i});
        g[v2 - 1].push_back({v1 - 1, i});
    }
 
    for (size_t i = 0; i < n; ++i)
        if (!used[i]) {
            dfs(i, -1);
            findComps(i);
        }
 
    cout << maxColor << "\n";
    int count = 0;
    map<int, int> correctSeq;
    for (size_t i = 0; i < n; ++i) {
        if (!correctSeq[numComps[i]])
            correctSeq[numComps[i]] = ++count;
        cout << correctSeq[numComps[i]] << ' ';
    }
    return 0;
}