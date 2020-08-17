#include <iostream>
#include <stack>

using namespace std;

struct node {
    long long key, priority, size;
    bool cycle = false, reverse = false;
    node *left_son, *right_son, *parent;

    explicit node (long long new_key) : key(new_key), priority((rand() << 15) | rand()),
                                        size(1LL),
                                        cycle(false),
                                        reverse(false),
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
    return (v == nullptr) ? 0LL : v->size;
}

void upd (node *&v) {
    if (v == nullptr) {
        return;
    }
    v->size = 1LL + get_size(v->left_son) + get_size(v->right_son);
}

void push (node *&v) {
    if (v == nullptr || !v->reverse) {
        return;
    }
    /** Reverse **/
    swap(v->left_son, v->right_son);
    v->reverse = false;
    if (v->left_son != nullptr) {
        v->left_son->reverse ^= true;
    }
    if (v->right_son != nullptr) {
        v->right_son->reverse ^= true;
    }
}

void super_push (node *&v) {
    if (v == nullptr) {
        return;
    }
    push(v);
    push(v->left_son);
    push(v->right_son);
}

pair<node *, node *> split (node *&root, long long index) {
    if (root == nullptr) {
        return {nullptr, nullptr};
    }
    super_push(root);
    long long root_size(get_size(root->left_son) + 1LL);

    if (root_size <= index) {
        auto res = split(root->right_son, index - root_size);
        root->right_son = res.first;
        if (res.first != nullptr) {
            res.first->parent = root;
        }
        if (res.second != nullptr) {
            res.second->parent = nullptr;
        }
        upd(root);
        return {root, res.second};
    } else {
        auto res = split(root->left_son, index);
        root->left_son = res.second;
        if (res.second != nullptr) {
            res.second->parent = root;
        }
        if (res.first != nullptr) {
            res.first->parent = nullptr;
        }
        upd(root);
        return {res.first, root};
    }
}

node *merge (node *&root_left, node *&root_right) {
    super_push(root_left);
    if (root_left == nullptr) {
        return root_right;
    }
    super_push(root_right);
    if (root_right == nullptr) {
        return root_left;
    }
    if (root_left->priority > root_right->priority) {
        root_left->right_son = merge(root_left->right_son, root_right);
        root_left->right_son->parent = root_left;
        upd(root_left);
        return root_left;
    } else {
        root_right->left_son = merge(root_left, root_right->left_son);
        root_right->left_son->parent = root_right;
        upd(root_right);
        return root_right;
    }
}

void push_to_root (node *&v) {
    if (v == nullptr) {
        return;
    }
    push_to_root(v->parent);
    super_push(v);
}

void print_treap (node *&root) {
    if (root == nullptr) {
        return;
    }
    super_push(root);
    print_treap(root->left_son);
    cout << root->key + 1 << ' ';
    //cout << root -> letter << ' ';
    print_treap(root->right_son);
}

long long min (long long a, long long b) {
    return a > b ? b : a;
}

long long max (long long a, long long b) {
    return a > b ? a : b;
}

node *find_root (node *v) {
    if (v == nullptr) {
        return nullptr;
    }
    int count = 0;
    while (v != nullptr && v->parent != nullptr) {
        v = v->parent;
        count++;
        if (count == 10000000) {
            exit(-1);
        }
    }
    return v;
}

long long find_number (node *v) {
    if (v == nullptr) {
        exit(-100);
    }
    push_to_root(v);
    node *help = v;
    long long res = get_size(help->left_son) + 1, count = 0;/// забыл + 1
    while (help != nullptr) {
        if (help->parent != nullptr) {
            if (help->parent->right_son == help) {
                res += get_size(help->parent->left_son) + 1;
            }
        }
        count++;
        if (count == 10000000) {
            exit(-100);
        }
        help = help->parent;
    }
    return res;
}

int main () {
    //freopen("markchain.in", "r", stdin);
    //freopen("markchain.out", "w", stdout);
    iostream::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int n, m, q;
    cin >> n >> m >> q;

    node *nodes[n];
    for (int i = 0; i < n; ++i) {
        nodes[i] = new node(i);
    }

    for (int i = 0; i < m; ++i) {
        long long left_city, right_city;
        cin >> left_city >> right_city;


        node *way_ahead = find_root(nodes[--left_city]);
        long long number1 = find_number(nodes[left_city]);
        node *way_back = find_root(nodes[--right_city]);
        long long number2 = find_number(nodes[right_city]);
        if (way_ahead == way_back) {
            way_ahead->cycle = true;
            continue;
        }

        if (number2 == 1 && number1 == get_size(way_ahead)) {
            merge(way_ahead, way_back);
        } else if (number1 == 1 && number2 == get_size(way_back)) {
            merge(way_back, way_ahead);
        } else if (number1 == 1) {
            way_ahead->reverse ^= true;
            merge(way_ahead, way_back);
        } else {
            way_back->reverse ^= true;
            merge(way_ahead, way_back);
        }
    }

    for (int i = 0; i < q; ++i) {
        char query;
        cin >> query;
        long long left_city, right_city;
        cin >> left_city >> right_city;
        if (query == '+') {
            node *way_ahead = find_root(nodes[--left_city]);
            long long number1 = find_number(nodes[left_city]);
            node *way_back = find_root(nodes[--right_city]);
            long long number2 = find_number(nodes[right_city]);
            if (way_ahead == way_back) {
                way_ahead->cycle = true;
                continue;
            }

            if (number2 == 1 && number1 == get_size(way_ahead)) {
                merge(way_ahead, way_back);
            } else if (number1 == 1 && number2 == get_size(way_back)) {
                merge(way_back, way_ahead);
            } else if (number1 == 1) {
                way_ahead->reverse ^= true;
                merge(way_ahead, way_back);
            } else {
                way_back->reverse ^= true;
                merge(way_ahead, way_back);
            }
        }
        if (query == '-') {
            node *way_ahead = find_root(nodes[--left_city]);
            long long number1 = find_number(nodes[left_city]), number2 = find_number(nodes[--right_city]);
            if (way_ahead->cycle) {
                way_ahead->cycle = false;
                if (abs(number1 - number2) > 1) {
                    continue;
                }
                auto save = split(way_ahead, min(number1, number2));
                merge(save.second, save.first);
            } else {
                split(way_ahead, min(number1, number2));
            }

            /*else if (!way_ahead->cycle) {
               auto v = split(way_ahead, n1 + 1);
           } else {
               // exit(-500);
               // return 0;
           }*/
//            print(way_ahead);
//            cout << '\n';
//            print(way_back);
//            cout << '\n';
        }
        if (query == '?') {
            if (nodes[--left_city] == nodes[--right_city]) {
                cout << "0\n";
                continue;
            }

            node *way_ahead = find_root(nodes[left_city]);
            long long number1 = find_number(nodes[left_city]);
            node *way_back = find_root(nodes[right_city]);
            long long number2 = find_number(nodes[right_city]);
            if (way_ahead == way_back) {
                if (way_ahead->cycle) {
                    cout << max(0LL,
                                min(abs(number1 - number2), get_size(way_ahead) - abs(number1 - number2)) -
                                1LL);
                } else {
                    cout << max(0LL, abs(number1 - number2) - 1LL);
                }
                cout << '\n';
            } else {
                cout << "-1\n";
            }
        }
    }
    // print(root);
    return 0;
}