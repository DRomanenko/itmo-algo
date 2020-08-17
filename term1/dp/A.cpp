
#include <bits/stdc++.h>

using namespace std;

#define ll long long

int main () {
    iostream::sync_with_stdio(false), cin.tie(nullptr);
    ifstream cin("input.txt");
    ofstream cout("output.txt");

    int n, k;

    cin >> n >> k;
    n -= 2;

    vector<int> cost(n + 2);
    for (int i = 1; i <= n; ++i) {
        cin >> cost[i];
    }

    vector<int> d(n + 2);
    vector<int> p(n + 2);

    ///База
    d[0] = 0;
    cost[0] = 0;
    cost[n + 1] = 0;
    p[0] = 0;

    //Переход
    for (int i = 1; i <= n + 1; ++i) {
        int saveMaxIndex = i - 1;
        for (int q = ((i < k) ? 0 : i - k); q <= i - 1; ++q) {
            if (d[q] > d[saveMaxIndex]) {
                saveMaxIndex = q;
            }
        }
        d[i] = d[saveMaxIndex] + cost[i];
        p[i] = saveMaxIndex;
    }

    cout << d[n + 1] << endl;

    // Восстановление ответа
    vector<int> ans;
    ans.push_back(n + 1);
    int q = n + 1;
    do {
        q = p[q];
        ans.push_back(q);
    } while (q > 0);
    cout << ans.size() - 1 << endl;
    for (int i = ans.size() - 1; i >= 0; --i) {
        cout << ans[i] + 1 << " ";
    }

    return 0;
}