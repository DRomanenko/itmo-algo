#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <algorithm>

using namespace std;

unsigned int n;

vector<vector<int>> sparseTable;

int op_min(int a, int b) {
    return a >= b ? b : a;
}

void build() {
    for (int i = 1; (1 << i) <= n; ++i) {
        sparseTable.resize(sparseTable.size() + 1);
        for (int j = 0; j <= n - (1 << i); ++j) {
            sparseTable[i].push_back(op_min(sparseTable[i - 1][j], sparseTable[i - 1][j + (1 << (i - 1))]));
        }
    }
}

int findSum(int l, int r) {
    int length = r - l + 1, level = (int)log2(length);
    return op_min(sparseTable[level][l], sparseTable[level][r - (1 << level) + 1]);
}

int main() {
    iostream::sync_with_stdio(0), cin.tie(0);
    //freopen("crypto.in", "r", stdin);
    //freopen("crypto.out", "w", stdout);
    //ifstream cin("crypto.in");
    //ofstream cout("crypto.out");

    unsigned int m;
    int a_0;
    cin >> n >> m >> a_0;
    int u_i, v_i;
    cin >> u_i >> v_i;

    sparseTable.resize(1);
    sparseTable[0].resize(n);

    sparseTable[0][0] = a_0;

    for (int i = 1; i < n; ++i) {
        int save = 23 * sparseTable[0][i - 1] + 21563;
        sparseTable[0][i] = (save < 0) ? save + (16714589): save % (16714589);
    }
    build();

    int ans = findSum(u_i - 1, v_i - 1);
    for (int i = 1; i < m; ++i) {
        //cout << i << ": " << u_i << " " << v_i << " " << ans << endl;
        u_i = ((17 * u_i + 751 + ans + 2 * i) % n) + 1;
        v_i = ((13 * v_i + 593 + ans + 5 * i) % n) + 1;
        ans = findSum(min(u_i - 1, v_i - 1), max(u_i - 1, v_i - 1));
    }

    cout << u_i << " " << v_i << " " << ans << endl;

    return 0;
}