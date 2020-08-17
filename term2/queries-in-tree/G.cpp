#include <iostream>
#include <vector>

using namespace std;

unsigned int FIRST_SIZE, SECOND_SIZE;
vector<vector<int>> graph, dp;
vector<int> depth, parent, left_border, right_border;
vector<bool> is_visited;

struct Node {
    long long sum_on_section = 0,  add = 0;
};

vector<Node> tree;

long long true_value (int v) {
    Node help = tree[v];
    if (tree[v].add != 0) {
        help.sum_on_section += tree[v].add;
    }
    return help.sum_on_section;
}

void push (int v, int l, int r) {
    if (l == r - 1) {
        tree[v].sum_on_section = true_value(v);
        return;
    }
    if (tree[v].add != 0) {
        tree[2 * v + 1].add += tree[v].add;
        tree[2 * v + 2].add += tree[v].add;
        tree[v].sum_on_section = true_value(v);
        tree[v].add = 0;
    }
}

long long op_sum (long long a, long long b) {
    return a + b;
}

void up_add (int v, int l, int r, int a, int b, long long add) {
    if (l >= b || r <= a) {
        return;
    }
    if (l >= a && r <= b) {
        tree[v].add += add;
        return;
    }
    push(v, l, r);
    int m = l + ((r - l) >> 1);
    up_add(2 * v + 1, l, m, a, b, add);
    up_add(2 * v + 2, m, r, a, b, add);
    tree[v].sum_on_section = op_sum(true_value(2 * v + 1), true_value(2 * v + 2));

}

long long query_sum (int v, int l, int r, int a, int b) {
    if (l >= b || r <= a) {
        return 0;
    }
    if (l >= a && r <= b) {
        return true_value(v);
    }
    push(v, l, r);
    int m = l + ((r - l) >> 1);
    return op_sum(query_sum(2 * v + 1, l, m, a, b), query_sum(2 * v + 2, m, r, a, b));
}

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

int boarder = 0;

void dfs (int v, int prev, int d) {
    left_border[v] = boarder++;
    is_visited[v] = true;
    depth[v] = d;
    parent[v] = prev;
    for (auto curr_v : graph[v]) {
        if (!is_visited[curr_v]) {
            dfs(curr_v, v, d + 1);
        }
    }
    right_border[v] = boarder - 1;
}

inline void pre_calc () {
    parent.assign(FIRST_SIZE, -1);
    depth.assign(FIRST_SIZE, -1);
    is_visited.assign(FIRST_SIZE, false);
    left_border.assign(FIRST_SIZE, 0), right_border.assign(FIRST_SIZE, 0);
    dfs(0, -1, 0);
    /*** If dp[i][q] == 0, then v don't have parent on 2^q ***/
    dp.assign(FIRST_SIZE, vector<int>(SECOND_SIZE, 0));

    for (int i = 1; i < FIRST_SIZE; ++i) {
        dp[i][0] = parent[i];
    }

    for (int i = 1; i < SECOND_SIZE; ++i) {
        for (int q = 1; q < FIRST_SIZE; ++q) {
            dp[q][i] = dp[dp[q][i - 1]][i - 1];
        }
    }
}

int lca (int v_1, int v_2) {
    /*** Инвариант: v_1 всегда ниже v_2 ***/
    if (depth[v_1] < depth[v_2]) {
        swap(v_1, v_2);
    }

    int hight = depth[v_1] - depth[v_2];
    /*** Вершину v_1 поднимаем на высоту hight ***/
    for (int i = SECOND_SIZE - 1; i >= 0; --i) {
        if ((1 << i) <= hight) {
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
            v_1 = dp[v_1][i], v_2 = dp[v_2][i];
        }
    }
    return dp[v_1][0];
}

int main () {
    iostream::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int n;
    cin >> n;

    FIRST_SIZE = n + 1, SECOND_SIZE = true_log2(n + 1) + 1;
    graph.resize(FIRST_SIZE);
    graph[0].push_back(1);
    graph[1].push_back(0);
    for (int i = 1; i < FIRST_SIZE - 1; ++i) {
        int v, u;
        cin >> v >> u;
        graph[v].push_back(u);
        graph[u].push_back(v);
    }

    pre_calc();
    tree.resize(4 * FIRST_SIZE);

    int m;
    cin >> m;
    for (int i = 0; i < m; ++i) {
        char command;
        int v, u, help;
        long long d;
        cin >> command;
        switch (command) {
            case '+':
                cin >> v >> u >> d;
                up_add(0, 0, FIRST_SIZE, left_border[v], left_border[v] + 1, d);
                up_add(0, 0, FIRST_SIZE, left_border[u], left_border[u] + 1, d);
                help = lca(v, u);
                up_add(0, 0, FIRST_SIZE, left_border[help], left_border[help] + 1, -d);
                up_add(0, 0, FIRST_SIZE, left_border[dp[help][0]], left_border[dp[help][0]] + 1, -d);
                break;
            case '?':
                cin >> v;
                cout << query_sum(0, 0, FIRST_SIZE, left_border[v], right_border[v] + 1) << '\n';
                break;
            default:
                exit(-1);
                break;
        }
    }
}