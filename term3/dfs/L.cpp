#include <iostream>
#include <vector>
#include <set>
#include <stack>
#include <algorithm>
#include <map>
 
using namespace std;
 
const size_t MAXS = 1E5 + 10;
set<int> dots;
vector<vector<pair<int, int>>> g(MAXS);
vector<bool> used(MAXS, false);
vector<int> tin(MAXS, INT32_MAX);
vector<int> d(MAXS, INT32_MAX);
size_t t = 0;
int maxNumComps = 0;
vector<int> numComps(2 * MAXS + 10, 0);
 
void findComps(int v, int color, int p) {
    used[v] = true;
    for (auto to : g[v]) {
        if (p == to.first)
            continue;
        if (used[to.first]) {
            if (tin[to.first] < tin[v]) {
                numComps[to.second] = color;
            }
        } else {
            if (d[to.first] < tin[v]) {
                numComps[to.second] = color;
                findComps(to.first, color, v);
            } else {
                numComps[to.second] = ++maxNumComps;
                findComps(to.first, maxNumComps, v);
            }
        }
    }
}
 
void findDotsConnect(int v, int p) {
    used[v] = true;
    tin[v] = t++;
    d[v] = tin[v];
    int qtyChild = 0;
    for (auto to : g[v]) {
        if (p == to.first)
            continue;
        if (used[to.first]) {
            d[v] = min(d[v], tin[to.first]);
        } else {
            findDotsConnect(to.first, v);
            ++qtyChild;
            d[v] = min(d[v], d[to.first]);
            if (p != -1 && d[to.first] >= tin[v])
                dots.insert(v);
        }
    }
    if (p == -1 && qtyChild > 1)
        dots.insert(v);
}
 
int main() {
    freopen("biconv.in", "r", stdin);
    freopen("biconv.out", "w", stdout);
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
        if (!used[i])
            findDotsConnect(i, -1);
 
    for (size_t i = 0; i < n; ++i)
        used[i] = false;
 
    for (size_t i = 0; i < n; ++i)
        if (!used[i])
            findComps(i, ++maxNumComps, -1);
 
    int count = 0;
    map<int, int> correctSeq;
    for (size_t i = 0; i < m; ++i)
        if (!correctSeq[numComps[i]])
            correctSeq[numComps[i]] = ++count;
    cout << count << '\n';
    for (size_t i = 0; i < m; ++i)
        cout << correctSeq[numComps[i]] << ' ';
    return 0;
}