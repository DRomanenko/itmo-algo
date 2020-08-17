#include <iostream>

using namespace std;

const long long inf = 9223372036854775807LL;

struct node {
    long long key, min_key = inf, max_key = -inf, sum = 0, size = 0;
    node *l_son, *r_son, *parent;

    explicit node (long long key) : key(key), min_key(key), max_key(key), sum(key), size(1LL), l_son(nullptr),
                                    r_son(nullptr),
                                    parent(nullptr) {
    }

    ~node () {
        delete l_son;
        delete r_son;
        delete parent;
    }
};

long long get_min (node *root) {
    return (root == nullptr) ? inf : root -> min_key;
}

long long get_max (node *root) {
    return (root == nullptr) ? -inf : root -> max_key;
}

long long min (long long a, long long b) {
    return (a > b) ? b : a;
}

long long max (long long a, long long b) {
    return (a < b) ? b : a;
}

long long get_sum (node *root) {
    return (root == nullptr) ? 0 : root -> sum;
}

long long get_size (node *root) {
    return (root == nullptr) ? 0 : root -> size;
}

void upd (node *v) {
    if (v == nullptr) {
        return;
    }
    v -> min_key = min(v -> key, min(get_min(v -> l_son), get_min(v -> r_son)));
    v -> max_key = max(v -> key, max(get_max(v -> l_son), get_max(v -> r_son)));
    v -> sum = v -> key + get_sum(v -> l_son) + get_sum(v -> r_son);
    v -> size = 1 + get_size(v -> l_son) + get_size(v -> r_son);
}

///          p                                          x
///         / \     -- Zig (Right Rotation) -->       /  \
///        x  T3                                     T1   r
///       / \                                            / \
///     T1  T2      <-- Zag (Left Rotation) --         T2   T3

node *zig (node *v) {
    node *parent = v -> l_son;
    v -> l_son = parent -> r_son;
    upd(v);
    parent -> r_son = v;
    upd(parent);
    return parent;
}

node *zag (node *v) {
    node *right_son = v -> r_son;
    v -> r_son = right_son -> l_son;
    upd(v);
    right_son -> l_son = v;
    upd(right_son);
    return right_son;
}

node *splay (node *root, int key) {
    /// Нет корня или ключ находится в корне.
    if (root == nullptr || root -> key == key) {
        return root;
    }

    if (root -> key > key) {
        /// Ключа нет в дереве
        if (root -> l_son == nullptr) {
            return root;
        }

        /// Zig-zig
        if (root -> l_son -> key > key) {
            root -> l_son -> l_son = splay(root -> l_son -> l_son, key);
            upd(root -> l_son);
            root = zig(root);
        } else if (root -> l_son -> key < key) { /// Zag-zig
            root -> l_son -> r_son = splay(root -> l_son -> r_son, key);
            upd(root -> l_son);
            if (root -> l_son -> r_son != nullptr) {
                root -> l_son = zag(root -> l_son);
            }
        }
        upd(root);
        return (root -> l_son == nullptr) ? root : zig(root);
    } else {
        /// Ключа нет в дереве
        if (root -> r_son == nullptr) {
            return root;
        }
        /// Zig-zag
        if (root -> r_son -> key > key) {
            root -> r_son -> l_son = splay(root -> r_son -> l_son, key);
            upd(root -> r_son);
            if (root -> r_son -> l_son != nullptr) {
                root -> r_son = zig(root -> r_son);
            }
        } else if (root -> r_son -> key < key) { /// Zag-zag
            root -> r_son -> r_son = splay(root -> r_son -> r_son, key);
            upd(root -> r_son);
            root = zag(root);
        }
        upd(root);
        return (root -> r_son == nullptr) ? root : zag(root);
    }
}

pair<node *, node *> split (node *v, long long key) {
    if (v == nullptr) {
        return {nullptr, nullptr};
    }
    v = splay(v, key);

    node *left = nullptr, *right = nullptr;
    if (v -> key >= key) {
        left = v -> l_son;
        v -> l_son = nullptr;
        upd(v);
        right = v;
    } else {
        right = v -> r_son;
        v -> r_son = nullptr;
        upd(v);
        left = v;
    }
    return {left, right};
}

node *merge (node *v_left, node *v_right) {
    if (v_left == nullptr) {
        return v_right;
    }
    if (v_right == nullptr) {
        return v_left;
    }
    v_right = splay(v_right, v_left -> key);
    v_right -> l_son = v_left;
    upd(v_right);
    return v_right;
}

node *insert (node *root, long long new_key) {
    if (root == nullptr) {
        return new node(new_key);
    }
    /// Сделаем ближайщий листовой узел корнем
    root = splay(root, new_key); //
    /// Если захочешь мультисет - убери
    if (root -> key == new_key) {
        return root;
    }

    pair<node *, node *> left_right = split(root, new_key);

    root = new node(new_key);
    root -> l_son = left_right . first;
    root -> r_son = left_right . second;
    upd(root);

    return root;
}

node *delete_key (node *root, long long key) {
    if (root == nullptr) {
        return root;
    }

    root = splay(root, key);

    if (root -> key != key) {
        return root;
    }

    node *l = root -> l_son;
    node *r = root -> r_son;

    if (l == nullptr && r == nullptr) {
        root = nullptr;
        upd(root);
        return root;
    }
    if (l == nullptr) {
        root = r;
        upd(root);
        return root;
    }
    if (r == nullptr) {
        root = l;
        upd(root);
        return root;
    }
    root = splay(r, get_min(r));
    root -> l_son = l;
    upd(root);
    return root;
}

bool exists (node *&root, long long key) {
    root = splay(root, key);
    return (root != nullptr && root -> key == key);
}

long long next (node *&root, long long key) {
    if (root == nullptr) {
        return -inf;
    }
    if (root -> key <= key) {
        return next(root -> r_son, key);
    }
    if (root -> l_son != nullptr && root -> l_son -> max_key > key) {
        return next(root -> l_son, key);
    }
    return root -> key;
}
/*
long long next (node *&root, long long key) {
    if (root == nullptr) {
        return -inf;
    }
    long long ans = -inf;
    pair<node *, node *> left_right = split(root, key);

    if (left_right . second != nullptr) {
        left_right . second = splay(left_right . second, key);
        ans = left_right . second -> key;
    }
    root = merge(left_right . first, left_right . second);
    return ans;
}
*/

long long prev (node *&root, int key) {
    if (root == nullptr) {
        return inf;
    }
    if (root -> key >= key) {
        return prev(root -> l_son, key);
    }
    if (root -> r_son != nullptr && root -> r_son -> min_key < key) {
        return prev(root -> r_son, key);
    }
    return root -> key;
}
/*
long long prev (node *&root, int key) {
    if (root == nullptr) {
        return inf;
    }
    pair<node *, node *> left_right = split(root, key);

    if (left_right . first != nullptr) {
        left_right . first = splay(left_right . first, key);
        root = merge(left_right . first, left_right . second);
        return left_right . first -> key;
    }
    return inf;
}
 */

long long sum (node *&root, long long l, long long r) {
    pair<node *, node *> left = split(root, l);
    pair<node *, node *> right = split(left . second, r + 1);
    long long ans = get_sum(right . first);
    root = merge(left . first, merge(right . first, right . second));
    return ans;
}

/// Можно превратить в kth_max -> root -> size - k + 1
long long kth_min (node *root, long long k) {
    if (root == nullptr || root -> size < k) {
        return -inf;
    }

    long long size = get_size(root -> l_son);
    if (size + 1 == k) {
        return root -> key;
    }

    if (size >= k) {
        return kth_min(root -> l_son, k);
    } else {
        return kth_min(root -> r_son, k - size - 1);
    }
}

void print_splay (node *root, int level = 0) {
    if (root != nullptr) {
        cout << root -> key << ":" << level << "    ";
        print_splay(root -> l_son, level + 1);
        print_splay(root -> r_son, level + 1);
    }
}

int main () {
    //freopen("markchain.in", "r", stdin);
    //freopen("markchain.out", "w", stdout);
    iostream::sync_with_stdio(false), cin . tie(nullptr), cout . tie(nullptr);
    node *root = nullptr;

    int n;
    cin >> n;
    long long save = 0, mod = 1e9;
    bool check = false;
    for (int i = 0; i < n; ++i) {
        char curr;
        cin >> curr;
        if (curr == '+') {
            long long new_key;
            cin >> new_key;
            if (check) {
                root = insert(root, (save + new_key) % mod);
                check = false;
            } else {
                root = insert(root, new_key);
            }
        }
        if (curr == '?') {
            long long l, r;
            cin >> l >> r;
            save = sum(root, l, r);
            cout << save << '\n';
            check = true;
        }
    }
    return 0;
}