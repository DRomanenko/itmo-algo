#include <iostream>
#include <vector>
#include <deque>
 
using namespace std;
 
const size_t MAXS = 26;
vector<vector<int>> g(MAXS);
vector<vector<int>> rg(MAXS);
vector<int> used(MAXS, 0);
vector<bool> beginS(MAXS, true);
vector<int> order;
bool hasCycle = false;
 
void findCycleAndTopSort(int v) {
    if (hasCycle) {
        return;
    }
    used[v] = 1;
    for (auto to : g[v]) {
        if (used[to] != 2) {
            if (used[to] == 0) {
                used[to] = 1;
                findCycleAndTopSort(to);
                if (hasCycle)
                    return;
            } else if (used[to] == 1) {
                hasCycle = true;
                return;
            }
        }
    }
    used[v] = 2;
    order.push_back(v);
}
 
int main() {
    freopen("tiv.in", "r", stdin);
    freopen("tiv.out", "w", stdout);
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
 
    size_t n;
    cin >> n;
    vector<string> words(n);
    for (size_t i = 0; i < n; ++i) {
        cin >> words[i];
    }
    if (words[0].size() != 1) {
        beginS[words[0][0] - 'a'] = false;
    }
    for (size_t i = 1; i < n; ++i) {
        if (words[i].size() != 1) {
            beginS[words[i][0] - 'a'] = false;
        }
        if (words[i - 1].size() > words[i].size() || words[i - 1] == words[i]) {
            cout << "NO\n";
            return 0;
        }
        if (words[i - 1].size() == words[i].size()) {
            size_t q = 0;
            while (q < words[i].size() && words[i - 1][q] == words[i][q])
                ++q;
            g[words[i - 1][q] - 'a'].push_back(words[i][q] - 'a');
            rg[words[i][q] - 'a'].push_back(words[i - 1][q] - 'a');
        }
    }
 
    int startV = -1;
    for (size_t i = 0; i < MAXS; ++i)
        if (beginS[i] && rg[i].empty())
            startV = i;
 
    for (size_t i = 0; i < MAXS; ++i) {
        if (startV == (int)i)
            continue;
        if (startV == -1 || hasCycle) {
            cout << "No\n";
            return 0;
        }
        if (used[i] != 2)
            findCycleAndTopSort(i);
    }
    findCycleAndTopSort(startV);
 
    cout << "Yes\n";
    vector<int> ans(MAXS);
    for (size_t i = 0; i < MAXS; ++i)
        ans[order[i]] = MAXS - 1 - i;
    for (size_t i = 0; i < MAXS; ++i)
        cout << ans[i] << ' ';
    return 0;
}