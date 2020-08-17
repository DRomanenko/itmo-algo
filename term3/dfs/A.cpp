#include <iostream>
#include <vector>
#include <stack>
 
using namespace std;
 
const size_t MAXS = 1E5;
vector<vector<int>> g(MAXS);
vector<int> used(MAXS, 0);
stack<int> s;
bool has_cycle = false;
 
void findCycle(int v) {
    if (has_cycle) {
        return;
    }
    used[v] = 1;
    for (auto to : g[v]) {
        if (used[to] != 2) {
            if (used[to] == 0) {
                used[to] = 1;
                findCycle(to);
            }
            if (used[to] == 1) {
                has_cycle = true;
                return;
            }
        }
    }
    used[v] = 2;
    s.push(v);
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
 
    for (size_t i = 0; i < n; ++i)
        if (used[i] != 2)
            findCycle(i);
 
    if (has_cycle) {
        cout << -1;
    } else {
        while (!s.empty()) {
            cout << s.top() + 1 << ' ';
            s.pop();
        }
    }
    return 0;
}