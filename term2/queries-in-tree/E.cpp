#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

vector<int> parent;

unsigned int FIRST_SIZE, SECOND_SIZE;
vector<vector<int>> dp;
vector<vector<bool>> q_visited;

vector<vector<int>> graph;
vector<int> depth;
vector<bool> used;

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

inline void dfs (int v, int p, int back) {
    used[v] = true;
    depth[v] = p;
    parent[v] = back;
    for (int curr_v : graph[v]) {
        if (!used[curr_v]) {
            dfs(curr_v, p + 1, v);
        }
    }
}

inline void pre_calc () {
    parent.resize(FIRST_SIZE);
    depth.assign(FIRST_SIZE, -1);
    used.assign(FIRST_SIZE, false);
    dfs(1, 0, 0);
    /*** If dp[i][q] == 0, then v don't have parent on 2^q ***/
    dp.assign(FIRST_SIZE, vector<int>(SECOND_SIZE, 0));
    q_visited.assign(FIRST_SIZE, vector<bool>(SECOND_SIZE, false));
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

void find_lca (int v_1, int v_2) {
    /*** Инвариант: v_1 всегда ниже v_2 ***/
    if (depth[v_1] < depth[v_2]) {
        swap(v_1, v_2);
    }

    int hight = depth[v_1] - depth[v_2];
    /*** Вершину v_1 поднимаем на высоту hight ***/
    for (int i = SECOND_SIZE - 1; i >= 0; --i) {
        if ((1 << i) <= hight) {
            q_visited[v_1][i] = true;
            v_1 = dp[v_1][i];
            hight -= (1 << i);
        }
    }
    /*** Если v_1 = v_2, то ответ на запрос v_1. **/
    if (v_1 == v_2) {
        return;
    }
    /***
       * А если v_1 ≠ v_2, то найдём такие вершины x и y,
       * такие что x ≠ y, x — предок v_1, y — предок v_2 и dp[x][0] = dp[y][0].
       * Тогда ответом на запрос будет dp[x][0].
    ***/
    for (int i = SECOND_SIZE - 1; i >= 0; --i) {
        if (dp[v_1][i] != dp[v_2][i]) {
            q_visited[v_1][i] = q_visited[v_2][i] = true;
            v_1 = dp[v_1][i], v_2 = dp[v_2][i];
        }
    }
    q_visited[v_1][0] = q_visited[v_2][0] = true;
}

int main () {
    //freopen("minonpath.in", "r", stdin);
    //freopen("minonpath.out", "w", stdout);
    iostream::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int n;
    cin >> n;
    ++n;

    FIRST_SIZE = n, SECOND_SIZE = true_log2(n) + 1;
    graph.assign(FIRST_SIZE, vector<int>(0));
    for (int i = 2; i < FIRST_SIZE; ++i) {
        int p, v;
        cin >> p >> v;
        graph[p].push_back(v);
        graph[v].push_back(p);
    }

    pre_calc();
    //print_dp();

    int m;
    cin >> m;
    for (int i = 0; i < m; ++i) {
        int v_1, v_2;
        cin >> v_1 >> v_2;
        find_lca(v_1, v_2);
    }

    for (int q = SECOND_SIZE - 1; q > 0; --q) {
        for (int i = FIRST_SIZE - 1; i >= 1; --i) {
            if (q_visited[i][q]) {
                q_visited[i][q - 1] = q_visited[dp[i][q - 1]][q - 1] = true;
            }
        }
    }

    int count = 0;
    for (int i = 2; i < q_visited.size(); ++i) {
        if (!q_visited[i][0]) {
            count++;
        }
    }
    cout << count << '\n';

    return 0;
}