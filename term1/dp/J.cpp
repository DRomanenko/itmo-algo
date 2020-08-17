#include <bits/stdc++.h>

using namespace std;

#define ll long long

int n, m;

string prof (int k) {
    string s;
    for (int i = 0; i < m; ++i, k /= 2) {
        s.push_back((char) (k % 2 + '0'));
    }
    return s;
}

int main () {
    iostream::sync_with_stdio(false), cin.tie(nullptr);
    ifstream cin("nice.in");
    ofstream cout("nice.out");
    cin >> n >> m;

    if (!n || !m) {
        cout << 0;
        return 0;
    }

    if (n < m) {
        swap(n, m);
    }
    vector<vector<ll>> connected(1 << m, vector<ll>(1 << m, 1)),
            d(n + 1, vector<ll>(1 << m));

    for (int i = 0; i < (1 << m); ++i) {
        d[1][i] = 1;
    }
    for (int i = (1 << m) - 1; i >= 0; --i) {
        for (int j = (1 << m) - 1; j >= 0; --j) {
            string s1 = prof(i), s2 = prof(j);
            for (int k = 0; k < m - 1; ++k) {
                if (s1[k] == s2[k] && s1[k] == s1[k + 1] && s1[k] == s2[k + 1]) {
                    connected[i][j] = connected[j][i] = 0;
                    break;
                }
            }
        }
    }
    for (int i = 2; i <= n; ++i) {
        for (int j = 0; j < (1 << m); ++j) {
            d[i][j] = 0;
            for (int k = 0; k < (1 << m); ++k) {
                d[i][j] += d[i - 1][k] * connected[j][k];
            }
        }
    }
    ll ans = 0;
    for (int i = 0; i < (1 << m); ++i) {
        ans += d[n][i];
    }

    cout << ans;
    return 0;
}