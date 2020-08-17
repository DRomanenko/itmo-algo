#include <iostream>
#include <vector>

using namespace std;

struct node {
    long key, priority, size, qty_zero;
    node *left_son, *right_son, *parent;

    explicit node (long long new_key) : key(new_key), priority((rand() << 15) | rand()),
                                        size(1LL), qty_zero(new_key == 0 ? 1 : 0),
                                        left_son(nullptr),
                                        right_son(nullptr),
                                        parent(nullptr) {
    }

    void set_key(long long new_key) {
        key = new_key;
        qty_zero = new_key == 0 ? 1 : 0;
    }

    ~node () {
        delete left_son;
        delete right_son;
        delete parent;
    }
};

long long get_size (node *v) {
    return (v == nullptr) ? 0 : v -> size;
}

long long get_qty_zero (node *v) {
    return (v == nullptr) ? 0 : v -> qty_zero;
}

void upd (node *v) {
    if (v == nullptr) {
        return;
    }
    v -> size = 1 + get_size(v -> left_son) + get_size(v -> right_son);
    v -> qty_zero = (v -> key == 0 ? 1 : 0) + get_qty_zero(v -> left_son) + get_qty_zero(v -> right_son);
}

pair<node *, node *> split (node *root, long long index) {
    if (root == nullptr) {
        return {nullptr, nullptr};
    }
    long long root_size(get_size(root -> left_son) + 1);

    if (root_size <= index) {
        auto res = split(root -> right_son, index - root_size);
        root -> right_son = res . first;
        upd(root);
        return {root, res . second};
    } else {
        auto res = split(root -> left_son, index);
        root -> left_son = res . second;
        upd(root);
        return {res . first, root};
    }
}

node *merge (node *root_left, node *root_right) {
    if (root_left == nullptr) {
        return root_right;
    }
    if (root_right == nullptr) {
        return root_left;
    }

    if (root_left -> priority > root_right -> priority) {
        root_left -> right_son = merge(root_left -> right_son, root_right);
        upd(root_left);
        return root_left;
    } else {
        root_right -> left_son = merge(root_left, root_right -> left_son);
        upd(root_right);
        return root_right;
    }
}

const int MAXN = 270000;
node * a[MAXN];
int l = 0;

node *insert (node *root, long long new_key) {
    a[l]->set_key(new_key);
    return merge(root, a[l++]);
}

long long find_true_pos (node *root, long long pos = 0) {
    if (root == nullptr || root -> qty_zero < 1) {
        return -1;
    }
    if (get_qty_zero(root -> left_son) == 0 && root -> qty_zero - get_qty_zero(root -> right_son) == 1) {
        return get_size(root -> left_son) + pos + 1;
    }
    if (get_qty_zero(root -> left_son) >= 1) {
        return find_true_pos(root -> left_son, pos);
    } else if (root -> qty_zero - get_qty_zero(root -> right_son) == 0) {
        return find_true_pos(root -> right_son, get_size(root -> left_son) + pos + 1);
    } else {
        return -1;
    }
}

node *insert_on_pos (node *root, long long pos, long long new_key) {
    auto left = split(root, pos - 1);
    auto right = split(left . second, 1);
    if (right . first != nullptr && right . first -> key == 0) {
        right . first -> key = new_key;
        right . first -> qty_zero -= (right . first -> key == 0 ? 0 : 1);
        root = merge(left . first, merge(right . first, right . second));
    } else {
        left = {left . first, merge(right . first, right . second)};
        long long true_pos = find_true_pos(left . second);
        if (true_pos != -1) {
            right = split(left . second, true_pos - 1);
            left . second = merge(right . first, split(right . second, 1) . second);
        }
        a[l]->set_key(new_key);
        root = merge(left . first, merge(a[l++], left . second));
    }
    return root;
}

void print_treap (node *root, vector<int> &ans) {
    if (root == nullptr) {
        return;
    }
    print_treap(root -> left_son, ans);
    ans . push_back(root -> key);
    //cout << root -> key << ' ';
    print_treap(root -> right_son, ans);
}

int main () {
    for (int i = 0; i < MAXN; ++i) {
        a[i] = new node(0);
    }
    //freopen("markchain.in", "r", stdin);
    //freopen("markchain.out", "w", stdout);
    iostream::sync_with_stdio(false), cin . tie(nullptr), cout . tie(nullptr);
    node *root = new node(0LL);

    int n, m;
    cin >> n >> m;

    for (int i = 1; i < m; ++i) {
        root = insert(root, 0LL);
    }

    vector<int> ans;
    //print_treap(root, ans);
    //cout << '\n';
    for (int i = 0; i < n; ++i) {
        long long pos;
        cin >> pos;
        root = insert_on_pos(root, pos, i + 1);

        /*print_treap(root, ans);
        int k = ans . size() - 1;
        while (ans[k] == 0) {
            k--;
        }
        cout << k + 1 << '\n';
        for (int j = 0; j < k + 1; ++j) {
            cout << ans[j] << ' ';
        }
        ans . resize(0);
        cout << '\n';*/
        //print_treap(root, ans);
        // cout << '\n';
    }
    print_treap(root, ans);
    int k = ans . size() - 1;
    while (ans[k] == 0) {
        k--;
    }
    cout << k + 1 << '\n';
    for (int j = 0; j < k + 1; ++j) {
        cout << ans[j] << ' ';
    }

    return 0;
}