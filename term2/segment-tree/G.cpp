#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

unsigned int n;

struct Vertex {
    int x = 0;
    int y = 0;
    int y_support = 0;
    int type = 1;
};

const long long inf = 9223372036854775807LL;

struct Node {
    long long q_max_on_section = -inf;
    long long q_index = 0;
    long long d_add = 0;
};

vector<Node> tree;
vector<int> arr;

Node true_value (long long v) {
    Node true_max = tree[v];
    if (tree[v].d_add != 0) {
        true_max.q_max_on_section += tree[v].d_add;
    }
    return true_max;
}

void push (long long v, long long l, long long r) {
    if (l == r - 1) {
        tree[v] = true_value(v);
        return;
    }
    if (tree[v].d_add != 0) {
        tree[2 * v + 1].d_add += tree[v].d_add;
        tree[2 * v + 2].d_add += tree[v].d_add;
        tree[v].q_max_on_section += tree[v].d_add;
        tree[v].d_add = 0;
    }
    return;
}

Node op_max (Node a, Node b) {
    return a.q_max_on_section < b.q_max_on_section ? b : a;
}

void up_add_max (long long v, int l, int r, int a, int b, long long add) {
    if (l >= b || r <= a) {
        return;
    }
    if (l >= a && r <= b) {
        tree[v].d_add += add;
        return;
    }
    push(v, l, r);
    long long m = l + ((r - l) >> 1);
    up_add_max(2 * v + 1, l, m, a, b, add);
    up_add_max(2 * v + 2, m, r, a, b, add);
    tree[v] = op_max(true_value(2 * v + 1), true_value(2 * v + 2));
    tree[v].d_add = 0;
}

Node query_max (long long v, int l, int r, int a, int b) {
    if (l >= b || r <= a) {
        Node fall = Node();
        return fall;
    }
    if (l >= a && r <= b) {
        return true_value(v);
    }
    push(v, l, r);
    long long m = l + ((r - l) >> 1);
    return op_max(query_max(2 * v + 1, l, m, a, b), query_max(2 * v + 2, m, r, a, b));
}

void treeBuild (long long v, int l, int r) {
    if (l == r) {
        return;
    }
    if (l == r - 1) {
        tree[v].q_max_on_section = 0;
        tree[v].q_index = l;
        tree[v].d_add = 0;
    } else {
        int m = l + ((r - l) >> 1);
        treeBuild(2 * v + 1, l, m);
        treeBuild(2 * v + 2, m, r);
        tree[v] = op_max(true_value(2 * v + 1), true_value(2 * v + 2));
    }
}

bool cmp (Vertex a, Vertex b) {
    return (a.x == b.x && a.y == b.y && a.type > b.type) || (a.x == b.x && a.y < b.y) || (a.x < b.x);
}

int binary_search (int key) {
    int left = -1;
    int right = arr.size();
    while (right > left + 1) {
        int middle = left + ((right - left) >> 1);
        if (arr[middle] < key) {
            left = middle;
        }
        else {
            right = middle;
        }
    }
    return right;
}

int main () {
    iostream::sync_with_stdio(false), cin.tie(nullptr);
    cin >> n;
    vector<Vertex> query(2 * n);
    set<int> S;
    for (int i = 0; i < n; ++i) {
        int x_1, y_1, x_2, y_2;
        cin >> x_1 >> y_1 >> x_2 >> y_2;
        Vertex a, b;
        a.x = x_1;
        a.y = y_1;
        a.y_support = y_2;
        a.type = 1;
        b.x = x_2;
        b.y = y_2;
        b.y_support = y_1;
        b.type = -1;

        S.insert(y_1);
        S.insert(y_2);

        query[2 * i] = a;
        query[2 * i + 1] = b;
    }

    for (auto u : S) {
        arr.push_back(u);
    }

    sort(arr.begin(), arr.end());
    sort(query.begin(), query.end(), cmp);

   /* for (auto u : query) {
        cout << u.x << " " << u.y << " " << u.x_support << " " << u.y_support << " " << u.type << "\n";
    }*/

    int true_size = arr.size();//2 * 1e5;

    tree.resize(4 * true_size + 1);
    treeBuild(0, 0, true_size);

    long long maxx = -inf;
    int save_y = -1, save_x = -1;
    for (int i = 0; i < query.size(); ++i) {
        if (query[i].type == 1) {
            int l = binary_search(query[i].y);
            int r = binary_search(query[i].y_support);
            up_add_max(0, 0, true_size, l, r + 1, 1);
        } else {
            int l = binary_search(query[i].y_support);
            int r = binary_search(query[i].y);
            up_add_max(0, 0, true_size, l, r + 1, -1);
        }

        Node save = query_max(0, 0, true_size, 0, true_size);
        if (maxx < save.q_max_on_section) {
            maxx = save.q_max_on_section;
            save_x = query[i].x;
            save_y = arr[save.q_index];
        }
    }

    cout << maxx << "\n" << save_x << " " << save_y;
    return 0;
}