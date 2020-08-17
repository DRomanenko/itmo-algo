#include <iostream>
#include <vector>

using namespace std;

const long long inf = 9223372036854775807LL;

struct Node {
    long long q_min_on_section = inf;
    long long q_index = 0;
    long long d_new_min = -inf;
};

unsigned int n;

vector<Node> tree;

Node true_value (long long v) {
    Node true_min = tree[v];
    if (tree[v].q_min_on_section < tree[v].d_new_min) {
        true_min.q_min_on_section = tree[v].d_new_min;
    }
    return true_min;
}

void push (long long v, long long l, long long r) {
    if (l == r - 1) {
        tree[v] = true_value(v);
        return;
    }
    if (tree[v].d_new_min != -inf) {
        if (tree[2 * v + 1].d_new_min < tree[v].d_new_min) {
            tree[2 * v + 1].d_new_min = tree[v].d_new_min;
        }
        if (tree[2 * v + 2].d_new_min < tree[v].d_new_min) {
            tree[2 * v + 2].d_new_min = tree[v].d_new_min;
        }
        tree[v] = true_value(v);
        tree[v].d_new_min = -inf;
    }
    return;
}

Node op_min (Node a, Node b) {
    return a.q_min_on_section > b.q_min_on_section ? b : a;
}

void up_set_min (long long v, int l, int r, int a, int b, long long new_min) {
    if (l >= b || r <= a) {
        return;
    }
    if (l >= a && r <= b) {
        if (true_value(v).q_min_on_section < new_min) {
            //tree[v].q_index = l;
            tree[v].d_new_min = new_min;
        }
        return;
    }
    push(v, l, r);
    long long m = l + ((r - l) >> 1);
    up_set_min(2 * v + 1, l, m, a, b, new_min);
    up_set_min(2 * v + 2, m, r, a, b, new_min);
    tree[v] = op_min(true_value(2 * v + 1), true_value(2 * v + 2));
    tree[v].d_new_min = -inf;
}

Node query_min (long long v, int l, int r, int a, int b) {
    if (l >= b || r <= a) {
        Node fall = Node();
        return fall;
    }
    if (l >= a && r <= b) {
        return true_value(v);
    }
    push(v, l, r);
    long long m = l + ((r - l) >> 1);
    return op_min(query_min(2 * v + 1, l, m, a, b), query_min(2 * v + 2, m, r, a, b));
}

void treeBuild (long long v, int l, int r) {
    if (l == r) {
        return;
    }
    if (l == r - 1) {
        tree[v].q_min_on_section = 0;
        tree[v].q_index = l;
        tree[v].d_new_min = -inf;
    } else {
        int m = l + ((r - l) >> 1);
        treeBuild(2 * v + 1, l, m);
        treeBuild(2 * v + 2, m, r);
        tree[v] = op_min(true_value(2 * v + 1), true_value(2 * v + 2));
    }
}

int main () {
    iostream::sync_with_stdio(false), cin.tie(nullptr);
    //freopen("rmq.in", "r", stdin);
    //freopen("rmq.out", "w", stdout);
    unsigned int m;
    cin >> n >> m;
    tree.resize(4 * n + 10);
    treeBuild(0, 0, n);
    for (int i = 0; i < m; ++i) {
        string query;
        cin >> query;
        unsigned int l, r;
        if (query == "defend") {
            unsigned int x;
            cin >> l >> r >> x;
            up_set_min(0, 0, n, l - 1, r, x);
        }
        if (query == "attack") {
            cin >> l >> r;
            Node ans = query_min(0, 0, n, l - 1, r);
            cout << ans.q_min_on_section << " " << ans.q_index + 1 << "\n";
        }
    }
    return 0;
}