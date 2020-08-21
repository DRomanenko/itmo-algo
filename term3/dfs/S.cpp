#include <iostream>
#include <vector>
 
using namespace std;
 
size_t MAXS = 300;
vector<vector<size_t>> g(MAXS);
vector<bool> used(MAXS, false);
vector<size_t> tin(MAXS, 0), tout(MAXS, 0);
vector<size_t> order(2 * MAXS);
 
void calcTime(size_t Size) {
    used.assign(MAXS, false);
    for (size_t i = 0; i < Size; ++i) {
        if (!used[order[i]])
            tin[order[i]] = i;
        used[order[i]] = true;
    }
 
    used.assign(MAXS, false);
    for (int i = Size - 1; i >= 0; --i) {
        if (!used[order[i]])
            tout[order[i]] = i;
        used[order[i]] = true;
    }
}
 
int main() {
    freopen("dfs.in", "r", stdin);
    freopen("dfs.out", "w", stdout);
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    size_t n, l;
    cin >> n >> l;
 
    for (size_t i = 0; i < l; ++i) {
        cin >> order[i];
        --order[i];
    }
 
    calcTime(l);
    int qty = 0;
    for (size_t v = 0; v < n; ++v) {
        if (tin[v] != tout[v]) {
            size_t timeCurrV = tin[v] + 1;
            used.assign(MAXS, false);
            while (timeCurrV < tout[v]) {
                size_t to = order[timeCurrV];
                used[to] = true;
                g[v].push_back(to);
                ++timeCurrV;
                while (timeCurrV <= tout[to]) {
                    if (!used[order[timeCurrV]]) {
                        if (order[timeCurrV] > to) {
                            g[v].push_back(order[timeCurrV]);
                            used[order[timeCurrV]] = true;
                            ++qty;
                        }
                    }
                    ++timeCurrV;
                }
                ++timeCurrV;
                ++qty;
            }
        }
    }
 
    cout << qty << '\n';
 
    for (size_t v = 0; v < n; ++v)
        for (int to : g[v])
            cout << v + 1 << ' ' << to + 1 << '\n';
 
    return 0;
}