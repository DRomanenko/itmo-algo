#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

unsigned int FIRST_SIZE, SECOND_SIZE;
vector<vector<int>> dp;

unsigned int true_log2 (unsigned int value) {
    if (value == 0) {
        return UINTMAX_MAX;
    }
    if (value == 1) {
        return 0;
    }
    unsigned int res = 0;
    while (value > 1) {
        value >>= 1;
        ++res;
    }
    return res;
}

inline void pre_calc (vector<int> &parent) {
    /*** If dp[i][q] == 0, then v don't have parent on 2^q ***/
    dp.assign(FIRST_SIZE, vector<int> (SECOND_SIZE, 0));
    for (int i = 1; i < FIRST_SIZE; ++i) {
        dp[i][0] = parent[i];
    }

    for (int i = 1; i < SECOND_SIZE; ++i) {
        for (int q = 1; q < FIRST_SIZE; ++q) {
            dp[q][i] = dp[dp[q][i - 1]][i - 1];
        }
    }
}

/*** Debug ***/
inline void print_dp () {
    for (int i = 1; i < FIRST_SIZE; ++i) {
        cout << i << ": ";
        for (int q = 0; q < SECOND_SIZE; ++q) {
            if (dp[i][q] != 0) {
                cout << dp[i][q] << ' ';
            }
        }
        cout << '\n';
    }
}


int main () {
    //freopen("fastminimization.in", "r", stdin);
    //freopen("fastminimization.out", "w", stdout);
    iostream::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int n;
    cin >> n;
    ++n;

    FIRST_SIZE = n, SECOND_SIZE = true_log2(n) + 1;

    vector<int> parent(FIRST_SIZE);
    for (int i = 1; i < FIRST_SIZE; ++i) {
        cin >> parent[i];
    }

    pre_calc(parent);
    print_dp();

    return 0;
}