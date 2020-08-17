#include <bits/stdc++.h>

using namespace std;

#define ll long long
const int MAX = (2E9);

int main () {
    iostream::sync_with_stdio(false), cin.tie(nullptr);

    string a, b;
    cin >> a >> b;

    vector<vector<unsigned int>> d(a.size() + 1, vector<unsigned int>(b.size() + 1));
    // d[i][q] - min число операций, чтобы получить из a[0...i-1] -> b[0...q-1];

    // База
    d[0][0] = 0;

    //Переход
    for (int i = 0; i < a.size() + 1; ++i) {
        for (int q = 0; q < b.size() + 1; ++q) {
            if (i == 0 && q == 0) {
                d[i][q] = 0;
                continue;
            }
            d[i][q] = MAX;
            if (i > 0 && q > 0 && a[i - 1] == b[q - 1]) {
                d[i][q] = d[i - 1][q - 1];
            }
            if (i > 0 && q > 0) {
                d[i][q] = min(d[i][q], d[i - 1][q - 1] + 1);
            }
            if (i > 0) {
                d[i][q] = min(d[i][q], d[i - 1][q] + 1);
            }
            if (q > 0) {
                d[i][q] = min(d[i][q], d[i][q - 1] + 1);
            }
        }
    }
    cout << d[a.size()][b.size()] << endl;

    return 0;
}