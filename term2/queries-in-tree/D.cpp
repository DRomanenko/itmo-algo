#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

const int MAX = INT32_MAX;

vector<int> parent;

unsigned int FIRST_SIZE, SECOND_SIZE;
vector<vector<int>> dp;
vector<int> depth;

unsigned int true_log2 (unsigned int value) {
    if (value == 0) {
        return MAX;
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

vector<int> Rank, parents;
void make_v (int v) {
    parents[v] = v;
    Rank[v] = 0;
}

int get (int v) {
    if (parents[v] != v) {
        parents[v] = get(parents[v]);
    }
    return parents[v];
}

void join (int x, int y) {
    x = get(x);
    y = get(y);
    if (x != y) {
        parents[y] = y;
        parents[x] = y;
    }
}

void insert (int number, int v) {
    make_v(number);
    depth[number] = depth[v] + 1;
    dp[number][0] = v;
    for (int i = 1; i < SECOND_SIZE; ++i) {
        dp[number][i] = dp[dp[number][i - 1]][i - 1];
    }
}

void erase (int v) {
    join(v, dp[v][0]);
}

int main () {
    //freopen("minonpath.in", "r", stdin);
    //freopen("minonpath.out", "w", stdout);
    iostream::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int m;
    cin >> m;


    FIRST_SIZE = m + 1, SECOND_SIZE = true_log2(m + 1) + 1;
    dp.assign(FIRST_SIZE, vector<int> (SECOND_SIZE, 0));
    depth.assign(FIRST_SIZE, -1), parent.resize(FIRST_SIZE);
    parents.resize(FIRST_SIZE), Rank.resize(FIRST_SIZE);
    make_v(1);
    parent[1] = 0;
    depth[1] = 0;

    int number = 1;
    for (int i = 0; i < m; ++i) {
        char c;
        int v, u;
        cin >> c;
        switch (c) {
            case '+':
                cin >> v;
                insert(++number, v);
                break;
            case '-':
                cin >> v;
                erase(v);
                break;
            case '?':
                cin >> v >> u;
                cout << get(find_lca(v, u)) << '\n';
                break;
        }
    }
    return 0;
}