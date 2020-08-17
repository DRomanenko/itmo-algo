#include <iostream>

using namespace std;

struct node {
    long key, priority, size;
    node *left_son, *right_son, *parent;

    explicit node (long long new_key) : key(new_key), priority((rand() << 15) | rand()),
                                        size(1LL),
                                        left_son(nullptr),
                                        right_son(nullptr),
                                        parent(nullptr) {
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

void upd (node *v) {
    if (v == nullptr) {
        return;
    }
    v -> size = 1 + get_size(v -> left_son) + get_size(v -> right_son);
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

node *insert (node *root, long long new_key) {
    return merge(root, new node(new_key));
}

void print_treap (node *root) {
    if (root == nullptr) {
        return;
    }
    print_treap(root -> left_son);
    cout << root -> key << ' ';
    print_treap(root -> right_son);
}

int main () {
    //freopen("markchain.in", "r", stdin);
    //freopen("markchain.out", "w", stdout);
    iostream::sync_with_stdio(false), cin . tie(nullptr), cout . tie(nullptr);
    node *root = new node(1LL);

    int n, m;
    cin >> n >> m;
    for (int i = 1; i < n; ++i) {
        root = insert(root, i + 1);
    }
    for (int i = 0; i < m; ++i) {
        long long l, r;
        cin >> l >> r;
        auto left = split(root, l - 1);
        auto right = split(left . second, r - l + 1);
        root = merge(right . first, merge(left . first, right . second));
        /*print_treap(root);
        cout << '\n';*/
    }
    print_treap(root);

    return 0;
}