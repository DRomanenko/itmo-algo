#include <iostream>
#include <vector>
#include <stack>
 
using namespace std;
 
const size_t MAXS = 1E5 + 10;
vector<vector<int>> g(MAXS);
vector<int> used(MAXS, 0);
vector<int> parent(MAXS, -1);
bool hasCycle = false;
int eCycle, bCycle = -1;
 
void findCycle(int v) {
    if (hasCycle)
        return;
    used[v] = 1;
    for (auto to : g[v])
        if (used[to] == 0) {
            parent[to] = v;
            findCycle(to);
            if (hasCycle)
                return;
        } else if (used[to] == 1) {
            eCycle = v;
            bCycle = to;
            hasCycle = true;
            return;
        }
    used[v] = 2;
}
 
int main() {
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
 
    size_t n, m;
    cin >> n >> m;
 
    for (size_t i = 0; i < m; ++i) {
        int v1, v2;
        cin >> v1 >> v2;
        g[v1 - 1].push_back(v2 - 1);
    }
 
    for (size_t i = 0; i < n; ++i) {
        if (hasCycle)
            break;
        if (!used[i])
            findCycle(i);
    }
 
    if (hasCycle) {
        stack<int> s;
 
        int v = eCycle;
        do {
            s.push(v);
            v = parent[v];
        } while (v != bCycle);
        s.push(bCycle);
 
        cout << "YES\n";
        while (!s.empty()) {
            cout << s.top() + 1 << ' ';
            s.pop();
        }
    } else {
        cout << "NO";
    }
    return 0;
}