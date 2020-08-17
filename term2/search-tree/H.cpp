#include <iostream>
#include <vector>
#include <bitset>

using namespace std;

struct node {
    char letter; /// key -> left_border
    long long priority, length, true_length;
    unsigned int diff_letter;
    node *left_son, *right_son, *parent;

    explicit node (char new_key, long long new_length) : letter(new_key), priority((rand() << 15) | rand()),
                                                         length(new_length),
                                                         true_length(new_length),
                                                         diff_letter(1 << (new_key - 'a')),
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

long long get_count_bit_mask (node *v) {
    if (v == nullptr) {
        return 0LL;
    }
    bitset<32> bits(v -> diff_letter);
    return bits.count();
}

long long get_length (node *v) {
    return (v == nullptr) ? 0 : v->length;
}

long long get_max_true_length (node *v) {
    return (v == nullptr) ? 0 : v->true_length;
}

long long get_bit_mask (node *v) {
    return (v == nullptr) ? 0 : v->diff_letter;
}

void upd (node *v) {
    if (v == nullptr) {
        return;
    }
    v->true_length = get_length(v) + get_max_true_length(v->left_son) + get_max_true_length(v->right_son);
    v->diff_letter = (1 << (v -> letter - 'a')) | get_bit_mask(v -> left_son) | get_bit_mask(v -> right_son);
}

pair<node *, node *> split (node *root, long long index) {
    if (root == nullptr) {
        return {nullptr, nullptr};
    }
    long long root_size(get_max_true_length(root->left_son) + get_length(root));

    if (root_size <= index) {
        auto res = split(root->right_son, index - root_size);
        root->right_son = res.first;
        upd(root);
        return {root, res.second};
    } else {
        auto res = split(root->left_son, index);
        root->left_son = res.second;
        upd(root);
        return {res.first, root};
    }
}

node *merge (node *root_left, node *root_right) {
    if (root_left == nullptr) {
        return root_right;
    }
    if (root_right == nullptr) {
        return root_left;
    }

    if (root_left->priority > root_right->priority) {
        root_left->right_son = merge(root_left->right_son, root_right);
        upd(root_left);
        return root_left;
    } else {
        root_right->left_son = merge(root_left, root_right->left_son);
        upd(root_right);
        return root_right;
    }
}

long long swift (node *v, long long r, long long sum) {
    if (v == nullptr) {
        return 0LL;
    }

    node *help = v;
    if (get_max_true_length(help->left_son) + get_length(help) >= r && get_max_true_length(help->left_son) < r) {
        return sum + get_max_true_length(help->left_son) + get_length(help);
    } else if (get_max_true_length(help) == r && get_max_true_length(help->left_son) < r) {
        return sum + get_max_true_length(help);
    } else if (get_max_true_length(help->left_son) >= r) {
        return swift(help->left_son, r, sum);
    } else if (get_max_true_length(help->left_son) + get_length(help) < r) {
        return swift(help->right_son, r - get_max_true_length(help->left_son) - get_length(help),
                     sum + get_max_true_length(help->left_son) + get_length(help));
    } else {
        exit(-21);
    }
}

node *find_left (node *root) {
    if (root == nullptr) {
        return nullptr;
    }
    node *help = root;
    while (help != nullptr && help->left_son != nullptr) {
        help = help->left_son;
    }
    return help;
}

node* consruct (unsigned int letter, long long length) {
    return (length > 0) ? new node(letter, length) : nullptr;
}

node* consruct (node* v, long long length) {
    return (v != nullptr && length > 0) ? new node(v -> letter, length) : nullptr;
}

node *insert_on_pos_letter (node *root, long long index, long long insert_length, char letter) {
    if (insert_length == 0) { /// ничего не вставляем
        return root;
    }

    auto left = split(root, index - 1); /// возможна проблема
    long long difference = index - get_max_true_length(left.first) - 1; /// сдвиг на длину отрезка, лежащего слева
    auto right = split(left.second, get_length(find_left(left.second))); /// здесь должна остаться только одна вершина

    if (difference < 0) { /// произошла ошибка
        exit(-1);
    }

    if (difference == 0) { /// элементы стоят рядом
        root = merge(left.first, merge(consruct(letter, insert_length), merge(right.first, right.second)));
    }

    if (difference > 0) { /// элементы не рядом, требуется разделение
        node *left_side = consruct(right.first, difference);
        node *right_side = consruct(right.first, get_length(right.first) - difference);
        root = merge(left.first, merge(left_side, merge(consruct(letter, insert_length), merge(right_side, right.second))));
    }

    return root;
}

node *delete_on_pos (node *root, long long index, long long cut_length) {
    if (cut_length == 0) { /// ничего не вырезаем
        return root;
    }

    auto left = split(root, index - 1); /// возможна проблема
    long long difference = index - get_max_true_length(left.first) - 1; /// сдвиг на длину отрезка, лежащего слева
    auto right = split(left.second, get_length(find_left(left.second))); /// здесь должна остаться только одна вершина

    if (difference < 0) {
        exit(-2);
    }

    if (difference >= 0 && difference + cut_length <= get_length(right.first)) { /// элементы стоят рядом, единый удаляемый подотрезок
        node *left_side = consruct(right.first, get_length(right.first) - cut_length);
        root = merge(left.first, merge(left_side, right.second));
    }

    if (difference >= 0 && difference + cut_length > get_length(right.first)) { /// элементы стоят рядом, кусочный удаляемый подотрезок
        node *left_side = consruct(right.first, difference);

        long long left_to_cut = cut_length - (get_length(right.first) - get_length(left_side)); /// разность отрезаемой длины с уже отрезанной

        auto mid = split(right.second, left_to_cut); /// возможно + difference
        auto tail = split(mid.second, get_length(find_left(mid.second))); /// одиночная правая вершина

        long long length_right_side = get_length(tail.first) -
                (cut_length - (get_length(right.first) - get_length(left_side))
                - get_max_true_length(mid.first));

        node *right_side = consruct(tail.first, length_right_side);
        root = merge(left.first, merge(left_side, merge(right_side, tail.second)));
    }

    return root;
}

long long query_diff_letter (node *&root, long long l, long long r) {
    if (l == r - 1) {
        return 1LL;
    }

    auto left = split(root, l);

    auto right = split(left.second, swift(left.second, r - get_max_true_length(left.first), 0LL));
    long long ans = get_count_bit_mask(right.first);
    root = merge(left.first, merge(right.first, right.second));
    return ans;
}

void print_treap (node *root) {
    if (root == nullptr) {
        return;
    }
    print_treap(root->left_son);
    for (int i = 0; i < root->length; ++i) {
        cout << root->letter;
    }
    //cout << root -> letter << ' ';
    print_treap(root->right_son);
}

int main () {
    //freopen("markchain.in", "r", stdin);
    //freopen("markchain.out", "w", stdout);
    iostream::sync_with_stdio(false), cin . tie(nullptr), cout . tie(nullptr);
    int n;
    cin >> n;

    node *root = nullptr;

    char op;
    cin >> op;
    if (op == '+') {
        char letter;
        int index, number;
        cin >> index >> number >> letter;
        root = consruct(letter, number);
    } else {
        return -20;
    }
    /*print_treap(root);
    cout << '\n';*/
    for (int i = 1; i < n; ++i) {
        cin >> op;
        if (op == '+') {
            char letter;
            int index, number;
            cin >> index >> number >> letter;
            root = insert_on_pos_letter(root, index, number, letter); /// т.к. перед буквой
        }
        if (op == '-') {
            int index, number;
            cin >> index >> number;
            root = delete_on_pos(root, index, number);
        }
        if (op == '?') {
            int l, r;
            cin >> l >> r;
            cout << query_diff_letter(root, l - 1, r) << '\n';
        }
        /*print_treap(root);
        cout << '\n';*/
    }

    return 0;
}
