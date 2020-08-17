#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <algorithm>

using namespace std;

const long long inf = 9223372036854775LL;

struct Node {
    long long left = inf;
    long long right = -inf;
};

unsigned int n;

vector<Node> tree;
vector<bool> arr;

Node op_merge (Node a, Node b) {
    Node merge;
    merge.left = a.left == inf ? b.left : a.left;
    merge.right = b.right == -inf ? a.right : b.right;
    return merge;
}

void up_set (int v, int l, int r, int pos, bool neww) {
    if (l == r - 1) {
        if (neww) {
            tree[v].left = inf;
            tree[v].right = -inf;
        } else {
            tree[v].left = l;
            tree[v].right = l;
        }
        return;
    }
    int m = l + ((r - l) >> 1);
    if (pos < m) {
        up_set(2 * v + 1, l, m, pos, neww);
    } else {
        up_set(2 * v + 2, m, r, pos, neww);
    }
    tree[v] = op_merge(tree[2 * v + 1], tree[2 * v + 2]);
}

void treeBuild (int v, int l, int r) {
    if (l == r) {
        return;
    }
    if (l == r - 1) {
        if (arr[l]) {
            tree[v].left = inf;
            tree[v].right = -inf;
        } else {
            tree[v].left = l;
            tree[v].right = l;
        }
    } else {
        int m = l + ((r - l) >> 1);
        treeBuild(2 * v + 1, l, m);
        treeBuild(2 * v + 2, m, r);
        tree[v] = op_merge(tree[2 * v + 1], tree[2 * v + 2]);
    }
}

int find (int v, int l, int r, int pos) {
    if (l == r - 1) {
        return l;
    }
    if ((tree[2 * v + 2].right < pos && tree[2 * v + 1].right < pos) ||
        (tree[2 * v + 1].left > n && tree[2 * v + 2].left > n)) {
        return -1;
    }
    int m = l + ((r - l) >> 1);
    if (tree[2 * v + 1].right >= pos) {
        find(2 * v + 1, l, m, pos);
    } else {
        find(2 * v + 2, m, r, pos);
    }
}

int main () {
    iostream::sync_with_stdio(false), cin.tie(nullptr);
    freopen("parking.in", "r", stdin);
    freopen("parking.out", "w", stdout);
    //iostream::sync_with_stdio(0), cin.tie(0);
    //ifstream cin("crypto.in");
    //ofstream cout("crypto.out");
    int m;
    cin >> n >> m;

    arr.resize(n + 10, false);
    tree.resize(4 * n + 10);
    treeBuild(0, 0, n);


    /*for(int i = 0; i < 2 * n; ++i) {
        printf("(%lld, %lld) ", tree[i].left, tree[i].right);
    }*/

    string s;
    for (int i = 0; i < m; ++i) {
        cin >> s;
        if (s == "enter") {
            int pos;
            cin >> pos;
            int true_pos = find(0, 0, n, pos - 1);
            int max_true_pos = true_pos == -1 ? find(0, 0, n, 0) : true_pos;
            cout << max_true_pos + 1 << "\n";
            up_set(0, 0, n, max_true_pos, true);
        }
        if (s == "exit") {
            int pos;
            cin >> pos;
            up_set(0, 0, n, pos - 1, false);
        }
    }

    return 0;
}