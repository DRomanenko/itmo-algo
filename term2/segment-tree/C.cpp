#include <iostream>
#include <vector>

using namespace std;

const long long inf = 9223372036854775807LL;

struct Node {
    long long min_on_section = inf;
    long long neww = -inf;
    long long add = 0;
};

unsigned int n;

vector<Node> tree;
vector<long long> arr;


long long true_value(int v) {
    Node p = tree[v];
    if (tree[v].neww != -inf) {
        p.min_on_section = tree[v].neww;
    }
    if (tree[v].add != 0) {
        p.min_on_section += tree[v].add;
    }
    return p.min_on_section;
}

void push(int v, int l, int r) {
    if (l == r - 1) {
        tree[v].min_on_section = true_value(v);
        return;
    }
    if (tree[v].neww != -inf) {
        tree[2 * v + 1].neww = tree[v].neww;
        tree[2 * v + 2].neww = tree[v].neww;
        tree[v * 2 + 1].add = 0;
        tree[v * 2 + 2].add = 0;
        tree[v].min_on_section = true_value(v);
        tree[v].neww = -inf;
    }
    if (tree[v].add != 0) {
        if (tree[v * 2 + 1].neww != -inf) {
            tree[v * 2 + 1].neww += tree[v].add;
        } else {
            tree[2 * v + 1].add += tree[v].add;
        }
        if (tree[v * 2 + 2].neww != -inf) {
            tree[v * 2 + 2].neww += tree[v].add;
        } else {
            tree[2 * v + 2].add += tree[v].add;
        }
        tree[v].min_on_section = true_value(v);
        tree[v].add = 0;
    }
    return;
}

long long op_min(long long a, long long b) {
    return a > b ? b : a;
}

void up_set(int v, int l, int r, int a, int b, long long neww) {
    if (l >= b || r <= a) {
        return;
    }
    if (l >= a && r <= b) {
        tree[v].neww = neww;
        tree[v].add = 0;
        return;
    }
    push(v, l, r);
    int m = l + ((r - l) >> 1);
    up_set(2 * v + 1, l, m, a, b, neww);
    up_set(2 * v + 2, m, r, a, b, neww);
    tree[v].min_on_section = op_min(true_value(2 * v + 1), true_value(2 * v + 2));
}

void up_add(int v, int l, int r, int a, int b, long long add) {
    if (l >= b || r <= a) {
        return;
    }
    if (l >= a && r <= b) {
        if (tree[v].neww != -inf) {
            tree[v].neww += add;
        } else {
            tree[v].add += add;
        }
        return;
    }
    push(v, l, r);
    int m = l + ((r - l) >> 1);
    up_add(2 * v + 1, l, m, a, b, add);
    up_add(2 * v + 2, m, r, a, b, add);
    tree[v].min_on_section = op_min(true_value(2 * v + 1), true_value(2 * v + 2));

}

long long query_min(int v, int l, int r, int a, int b) {
    //push(v, l, r);
    if (l >= b || r <= a) {
        return inf;
    }
    if (l >= a && r <= b) {
        return true_value(v);
    }
    push(v, l, r);
    int m = l + ((r - l) >> 1);
    return op_min(query_min(2 * v + 1, l, m, a, b), query_min(2 * v + 2, m, r, a, b));
}

void treeBuild(int v, int l, int r) {
    if (l == r) {
        return;
    }
    if (l == r - 1) {
        tree[v].min_on_section = arr[l];
    } else {
        int m = l + ((r - l) >> 1);
        treeBuild(2 * v + 1, l, m);
        treeBuild(2 * v + 2, m, r);
        tree[v].min_on_section = op_min(true_value(2 * v + 1), true_value(2 * v + 2));
    }
}

int main() {
    //iostream::sync_with_stdio(0), cin.tie(0);
    cin >> n;

    arr.resize(n + 10);
    for (int i = 0; i < n; ++i) {
        long long a;
        cin >> a;
        arr[i] = a;
    }

    tree.resize(4 * n + 10);
    treeBuild(0, 0, n);

    /*for(int i = 0; i < 2 * n; ++i) {
        cout << tree[i].min_on_section << " ";
    }*/

    string s;
    while(cin >> s) {
        if (s == "set") {
            int i, j;
            long long x;
            cin >> i >> j >> x;
            up_set(0, 0, n, i - 1, j, x);
        }
        if (s == "add") {
            int i, j;
            long long x;
            cin >> i >> j >> x;
            up_add(0, 0, n, i - 1, j, x);
        }
        if (s == "min") {
            int i, j;
            cin >> i >> j;
            cout << query_min(0, 0, n, i - 1, j) << '\n';
        }
        /*for(int i = 0; i < 3 * n; ++i) {
            cout << tree[i].min_on_section << " ";
        }*/
    }

    return 0;
}