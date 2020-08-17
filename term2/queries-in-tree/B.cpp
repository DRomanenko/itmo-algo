#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

vector<int> parent;

unsigned int FIRST_SIZE, SECOND_SIZE;
vector<vector<int>> dp;

vector<vector<int>> g;
int timer;
vector<int> depth;

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

inline void dfs (int v, int p) {
    depth[v] = p;
    for (int curr_v : g[v]) {
        dfs(curr_v, p + 1);
    }
}

inline void pre_calc () {
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
    depth.assign(FIRST_SIZE, -1);
    dfs(1, 0);
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

int find_lca (int v_1, int v_2) {
    /*** Инвариант: v_1 всегда ниже v_2 ***/
    if (depth[v_1] < depth[v_2]) {
        swap(v_1, v_2);
    }

    int hight = depth[v_1] - depth[v_2];
    /*** Вершину v_1 поднимаем на высоту hight ***/
    for (int i = SECOND_SIZE - 1; i >= 0; --i) {
        if ((1 << i) <= depth[v_1] - depth[v_2]) {
            v_1 = dp[v_1][i];
            hight -= (1 << i);
        }
    }
    /*** Если v_1 = v_2, то ответ на запрос v_1. **/
    if (v_1 == v_2) {
        return v_1;
    }
    /*** 
       * А если v_1 ≠ v_2, то найдём такие вершины x и y, 
       * такие что x ≠ y, x — предок v_1, y — предок v_2 и dp[x][0] = dp[y][0]. 
       * Тогда ответом на запрос будет dp[x][0].
    ***/
    for (int i = SECOND_SIZE - 1; i >= 0; --i) {
        if (dp[v_1][i] != dp[v_2][i]) {
            v_1 = dp[v_1][i];
            v_2 = dp[v_2][i];
        }
    }
    return dp[v_1][0];
}

int main () {
    //freopen("fastminimization.in", "r", stdin);
    //freopen("fastminimization.out", "w", stdout);
    iostream::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int n;
    cin >> n;
    ++n;

    FIRST_SIZE = n, SECOND_SIZE = true_log2(n) + 1;
    g.assign(n, vector<int> (0));
    parent.resize(FIRST_SIZE);
    parent[1] = 0;
    for (int i = 2; i < FIRST_SIZE; ++i) {
        cin >> parent[i];
        g[parent[i]].push_back(i);
    }

    pre_calc();

    int m;
    cin >> m;
    for (int i = 0; i < m; ++i) {
        int v_1, v_2;
        cin >> v_1 >> v_2;
        cout << find_lca(v_1, v_2) << '\n';
    }

    return 0;
}