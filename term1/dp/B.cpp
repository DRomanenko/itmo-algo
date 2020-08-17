#include <bits/stdc++.h>

using namespace std;

#define ll long long

int main () {
    iostream::sync_with_stdio(false), cin.tie(nullptr);
    ifstream cin("input.txt");
    ofstream cout("output.txt");

    unsigned int n, m;

    cin >> n >> m;

    vector<vector<int>> cost(n, vector<int> (m));
    for (int i = 0; i < n; ++i) {
        for (int q = 0; q < m; ++q) {
            cin >> cost[i][q];
        }
    }

    vector<vector<int>> d(n, vector<int> (m));
    vector<vector<char>> p(n, vector<char> (m));

    //Переход
    for (int i = 0; i < n; ++i) {
        for (int q = 0; q < m; ++q) {
            if (i > 0 && q > 0) {
                if (d[i - 1][q] > d[i][q - 1]) {
                    d[i][q] = d[i - 1][q];
                    p[i][q] = 'D';
                } else {
                    d[i][q] = d[i][q - 1];
                    p[i][q] = 'R';
                }
            } else if (i > 0) {
                d[i][q] = d[i - 1][q];
                p[i][q] = 'D';
            } else if (q > 0) {
                d[i][q] = d[i][q - 1];
                p[i][q] = 'R';
            }
            d[i][q] += cost[i][q];
        }
    }
    cout << d[n - 1][m - 1] << endl;


    // Восстановление ответа
    vector<char> ans;
    int i = n - 1, q = m - 1;
    while (i > 0 || q > 0) {
        if (i >= 0 && p[i][q] == 'R') {
            ans.push_back('R');
            q--;
        } else if (q >= 0 && p[i][q] == 'D') {
            ans.push_back('D');
            i--;
        }
    }
    for (i = ans.size() - 1; i >= 0; --i) {
        cout << ans[i];
    }

    return 0;
}