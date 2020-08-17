#include <iostream>
#include <vector>

using namespace std;

const int white = 0, black = 1;

struct Node {
    int color_l = -1;
    int color_r = -1;
    long long qty_white = 0;
    long long qty_black = 0;
    long long sum_length_white = 0;
    long long sum_length_black = 0;
    int new_color = -1;
};

Node neutral_element;

unsigned int n;
vector<Node> tree;

Node true_value(int v) {
    Node p = tree[v];
    if (tree[v].new_color == white) {
        p.color_l = white;
        p.color_r = white;
        p.qty_white = 1;
        p.qty_black = 0;
        p.sum_length_white = tree[v].sum_length_white + tree[v].sum_length_black;
        p.sum_length_black = 0;
        p.new_color = white;
    }
    if (tree[v].new_color == black) {
        p.color_l = black;
        p.color_r = black;
        p.qty_white = 0;
        p.qty_black = 1;
        p.sum_length_white = 0;
        p.sum_length_black = tree[v].sum_length_white + tree[v].sum_length_black;
        p.new_color = black;
    }
    return p;
}

void push(int v, int l, int r) {
    if (l == r - 1) {
        tree[v] = true_value(v);
        return;
    }
    if (tree[v].new_color == white || tree[v].new_color == black) {
        tree[2 * v + 1].new_color = tree[v].new_color;
        tree[2 * v + 2].new_color = tree[v].new_color;
        tree[v] = true_value(v);
        tree[v].new_color = -1;
    }
    return;
}

Node op_merge(Node left, Node right) {
    Node merge;
    merge.color_l = left.color_l;
    merge.color_r = right.color_r;
    merge.qty_white = left.qty_white + right.qty_white - ((left.color_r == right.color_l && left.color_r == white) ? 1LL : 0LL);
    merge.qty_black = left.qty_black + right.qty_black - ((left.color_r == right.color_l && left.color_r == black) ? 1LL : 0LL);
    merge.sum_length_white = left.sum_length_white + right.sum_length_white;
    merge.sum_length_black = left.sum_length_black + right.sum_length_black;
    merge.new_color = -1;
    return merge;
}

void up_set(int v, int l, int r, int a, int b, int new_color) {
    if (l >= b || r <= a) {
        return;
    }
    if (l >= a && r <= b) {
        tree[v].new_color = new_color;
        return;
    }
    push(v, l, r);
    int m = l + ((r - l) >> 1);
    up_set(2 * v + 1, l, m, a, b, new_color);
    up_set(2 * v + 2, m, r, a, b, new_color);
    tree[v] = op_merge(true_value(2 * v + 1), true_value(2 * v + 2));
}

Node query_section (int v, int l, int r, int a, int b) {
    if (l >= b || r <= a) {
        return neutral_element;
    }
    if (l >= a && r <= b) {
        return true_value(v);
    }
    push(v, l, r);
    int m = l + ((r - l) >> 1);
    return op_merge(query_section(2 * v + 1, l, m, a, b), query_section(2 * v + 2, m, r, a, b));
}

void treeBuild(int v, int l, int r) {
    if (l == r) {
        return;
    }
    if (l == r - 1) {
        tree[v].color_l = white;
        tree[v].color_r = white;
        tree[v].qty_white = 1;
        tree[v].qty_black = 0;
        tree[v].sum_length_white = 1;
        tree[v].sum_length_black = 0;
        tree[v].new_color = -1;
    } else {
        int m = l + ((r - l) >> 1);
        treeBuild(2 * v + 1, l, m);
        treeBuild(2 * v + 2, m, r);
        tree[v] = op_merge(true_value(2 * v + 1), true_value(2 * v + 2));
    }
}

int main () {
    iostream::sync_with_stdio(false), cin.tie(nullptr);
    //freopen("parking.in", "r", stdin);
    //freopen("parking.out", "w", stdout);
    const int size = 1000000;
    //const int size = 10;
    tree.resize(4 * size + 10);
    treeBuild(0, 0, size);
    cin >> n;
    for (int i = 0; i < n; ++i) {
        char new_color;
        cin >> new_color;
        int pos, l;
        cin >> pos >> l;
        //pos--;
        /*pos--;
        pos += 499999;*/
        pos += size / 2;

        up_set(0, 0, size, pos, pos + l, (new_color == 'W') ? white : black);
        Node v = query_section(0, 0, size, 0, size);
        cout << v.qty_black << " " << v.sum_length_black << "\n";
    }

    return 0;
}