#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

const long long inf = 9223372036854775807LL;

long long n;

struct Node {
    long long sum;
    long long q_max_high;
    long long d_set;
    long long l;
    long long r;
    Node *l_son, *r_son;

    Node (long long a, long long b) : sum(0), q_max_high(0), d_set(-inf), l_son(nullptr), r_son(nullptr) {
        l = a;
        r = b;
    }
};

Node *true_value (Node *v) {
    Node *true_val = v;
    if (v -> d_set != -inf) {
        true_val -> sum = (v -> r - v -> l) * v -> d_set;
        //true_val -> q_max_high = max(v -> q_max_high, true_val -> sum);
        true_val -> q_max_high = max(0LL, true_val -> sum);
    }
    return true_val;
}

void push (Node *v) {
    if (v -> l == v -> r - 1) {
        v = true_value(v);
        v -> d_set = -inf;
        return;
    }

    long long m = v -> l + ((v -> r - v -> l) >> 1);
    if (v -> l_son == nullptr) {
        v -> l_son = new Node(v -> l, m);
        v -> l_son -> sum = (true_value(v) -> sum >> 1);
        v -> l_son -> q_max_high = max(0LL, (true_value(v) -> sum >> 1)); // ?????????????????????????????

    }
    if (v -> r_son == nullptr) {
        v -> r_son = new Node(m, v -> r);
        v -> r_son -> sum = true_value(v) -> sum - v -> l_son -> sum;
        v -> r_son -> q_max_high = max(0LL, v -> r_son -> sum); // ?????????????????????????????
    }

    if (v -> d_set != -inf) {
        v -> l_son -> d_set = v -> d_set;
        v -> r_son -> d_set = v -> d_set;
        v = true_value(v);
        v -> d_set = -inf;
    }
    return;
}

long long op_merge_sum (Node *a, Node *b) {
    return a -> sum + b -> sum;
}

long long op_merge_max (Node *a, Node *b) {
    return max(a -> sum + b -> sum, max(a -> q_max_high, a -> sum + b -> q_max_high));
}

void up_set (Node *v, long long l, long long r, long long a, long long b, long long new_set) {
    if (l >= b || r <= a) {
        return;
    }
    if (l >= a && r <= b) {
        v -> d_set = new_set;
        return;
    }
    push(v);
    long long m = l + ((r - l) >> 1);

    up_set(v -> l_son, l, m, a, b, new_set);
    up_set(v -> r_son, m, r, a, b, new_set);

    v -> sum = op_merge_sum(true_value(v -> l_son), true_value(v -> r_son));
    v -> q_max_high = op_merge_max(true_value(v -> l_son), true_value(v -> r_son));
    v -> d_set = -inf;
}

long long query_find (Node *v, long long l, long long r, long long high) {
    if (true_value(v) -> q_max_high <= high) {
        return r;
    }
    if (l == r - 1 && true_value(v) -> q_max_high > high) {
        return l;
    }
    push(v);
    long long m = l + ((r - l) >> 1);
    if (true_value(v -> l_son) -> q_max_high > high) {
        return query_find(v -> l_son, l, m, high);
    } else {
        return query_find(v -> r_son, m, r, high - true_value(v -> l_son) -> sum);
    }
}

int main () {
    iostream::sync_with_stdio(false), cin.tie(nullptr);
    cin >> n;
    char query;
    Node *root = new Node(0, n);

    while (cin >> query && query != 'E') {
        if (query == 'I') {
            long long a, b, new_d;
            cin >> a >> b >> new_d;
            up_set(root, 0, n, a - 1, b, new_d);
        }
        if (query == 'Q') {
            long long high;
            cin >> high;
            cout << query_find(root, 0, n, high) << '\n';
        }
    }
    delete root;
    return 0;
}