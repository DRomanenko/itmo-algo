#include <bits/stdc++.h>

using namespace std;

const long long inf = 9223372036854775807LL;

struct Node {
    long long a_0_0 = 1;
    long long a_0_1 = 0;
    long long a_1_0 = 0;
    long long a_1_1 = 1;
};

unsigned int n;
long long r;
Node zero;

vector<Node> tree;
vector<Node> arr;

Node op_composition(Node matrix_left, Node matrix_right) {
    Node comp;
    comp.a_0_0 = ((matrix_left.a_0_0 * matrix_right.a_0_0) % r + (matrix_left.a_0_1 * matrix_right.a_1_0) % r) % r; // checked
    comp.a_0_1 = ((matrix_left.a_0_0 * matrix_right.a_0_1) % r + (matrix_left.a_0_1 * matrix_right.a_1_1) % r) % r; // checked
    comp.a_1_0 = ((matrix_left.a_1_0 * matrix_right.a_0_0) % r + (matrix_left.a_1_1 * matrix_right.a_1_0) % r) % r; // checked
    comp.a_1_1 = ((matrix_left.a_1_0 * matrix_right.a_0_1) % r + (matrix_left.a_1_1 * matrix_right.a_1_1) % r) % r; // checked
    return comp;
}

Node query_comp(int v, int l, int r, int a, int b) {
    if (l >= b || r <= a) {
        return zero;
    }
    if (l >= a && r <= b) {
        return tree[v];
    }
    int m = l + ((r - l) >> 1);
    return op_composition(query_comp(2 * v + 1, l, m, a, b), query_comp(2 * v + 2, m, r, a, b));
}

void treeBuild(int v, int l, int r) {
    if (l == r) {
        return;
    }
    if (l == r - 1) {
        tree[v] = arr[l];
    } else {
        int m = l + ((r - l) >> 1);
        treeBuild(2 * v + 1, l, m);
        treeBuild(2 * v + 2, m, r);
        tree[v] = op_composition(tree[2 * v + 1], tree[2 * v + 2]);
    }
}

int main() {
//iostream::sync_with_stdio(0), cin.tie(0);
    freopen("crypto.in", "r", stdin);
    freopen("crypto.out", "w", stdout);
    iostream::sync_with_stdio(0), cin.tie(0);
//ifstream cin("crypto.in");
//ofstream cout("crypto.out");
    int m;
    cin >> r >> n >> m;

    arr.resize(n + 10);
    for (int i = 0; i < n; ++i) {
        long long a_0_0, a_0_1, a_1_0, a_1_1;
        cin >> a_0_0 >> a_0_1 >> a_1_0 >> a_1_1;
        arr[i].a_0_0 = a_0_0;
        arr[i].a_0_1 = a_0_1;
        arr[i].a_1_0 = a_1_0;
        arr[i].a_1_1 = a_1_1;
    }
    zero.a_0_0 = 1;
    zero.a_0_1 = 0;
    zero.a_1_0 = 0;
    zero.a_1_1 = 1;

    tree.resize(4 * n + 10);
    treeBuild(0, 0, n);

/*for(int i = 0; i < 2 * n; ++i) {
cout « tree[i].min_on_section « " ";
}*/

    for (int i = 0; i < m; ++i) {
        int l, r;
        cin >> l >> r;
        Node v = query_comp(0, 0, n, l - 1, r);
        cout << v.a_0_0 << " " << v.a_0_1 << "\n";
        cout << v.a_1_0 << " " << v.a_1_1 << "\n";
        cout << "\n";
    }

    return 0;
}