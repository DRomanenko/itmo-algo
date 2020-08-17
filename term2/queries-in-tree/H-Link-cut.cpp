#include <iostream>
#include <vector>

using namespace std;

#define NOT_FLIPED false
#define FLIPED true

struct Node {
    long long key, size = 0;
    Node *l_son, *r_son, *parent, *path_parent;
    bool fliped;

    explicit Node (long long key) : key(key), size(1LL), fliped(NOT_FLIPED),
                                    l_son(nullptr), r_son(nullptr),
                                    parent(nullptr), path_parent(nullptr) {
    }
};

Node *get_parent (Node *v) {
    return v ? v->parent : nullptr;
}

Node *get_grand_parent (Node *v) {
    return get_parent(get_parent(v));
}

int is_root (Node *v) {
    return (v->parent == nullptr) || (v->parent->l_son != v && v->parent->r_son != v);
}

int is_left_child (Node *v) {
    return (v->parent->l_son == v);
}

int is_right_child (Node *v) {
    return (v->parent->r_son == v);
}

void push_flip (Node *v) {
    if (v->fliped) {
        swap(v->l_son, v->r_son);
        v->fliped = NOT_FLIPED;
        if (v->l_son) {
            v->l_son->fliped ^= FLIPED;
        }
        if (v->r_son) {
            v->r_son->fliped ^= FLIPED;
        }
    }
}

void zag (Node *v) {
    Node *right_son = v->r_son, *parent = v->parent;

    if (parent != nullptr) {
        if (is_left_child(v)) {
            parent->l_son = right_son;
        } else if (is_right_child(v)) {
            parent->r_son = right_son;
        }
    }

    v->r_son = right_son->l_son;
    right_son->l_son = v;
    right_son->parent = parent;
    v->parent = right_son;

    if (v->r_son != nullptr) {
        v->r_son->parent = v;
    }
}

void zig (Node *v) {
    Node *left_son = v->l_son, *parent = v->parent;

    if (parent != nullptr) {
        if (is_left_child(v)) {
            parent->l_son = left_son;
        } else if (is_right_child(v)) {
            parent->r_son = left_son;
        }
    }

    v->l_son = left_son->r_son;
    left_son->r_son = v;
    left_son->parent = parent;
    v->parent = left_son;

    if (v->l_son != nullptr) {
        v->l_son->parent = v;
    }
}

void splay (Node *x) {
    Node *parent, *grand_parent;
    while (!is_root(x)) {
        parent = get_parent(x);
        grand_parent = get_grand_parent(x);

        /*** Unflip if we have any node fliped (from the grandparent to the child) ***/
        if (grand_parent) {
            push_flip(grand_parent);
        }
        push_flip(parent);
        push_flip(x);

        /*** Parent is root ***/
        if (is_root(parent)) {
            x->path_parent = parent->path_parent;
            parent->path_parent = nullptr;
        } else if (is_root(grand_parent)) { /*** Grand_parent is root ***/
            x->path_parent = grand_parent->path_parent;
            grand_parent->path_parent = nullptr;
        }

///          p                                          x
///         / \     -- Zig (Right Rotation) -->       /  \
///        x  T3                                     T1   p
///       / \                                            / \
///     T1  T2      <-- Zag (Left Rotation) --         T2   T3
        if (is_left_child(x)) {
            if (grand_parent == nullptr) {
                zig(parent);
                return;

            }

            if (is_left_child(parent)) { /* x is left of parent(x) and parent(x) is left of g(x) */
                zig(get_grand_parent(x)), zig(get_parent(x));
            } else { /* x is left of parent(x) and parent(x) is right of g(x) */
                zig(get_parent(x)), zag(get_parent(x));
            }
        } else {
            if (grand_parent == nullptr) {
                zag(parent);
                return;
            }

            if (is_right_child(parent)) { /* x is right of parent(x) and parent(x) is right of g(x) */
                zag(get_grand_parent(x)), zag(get_parent(x));
            } else { /* x is right of parent(x) and parent(x) is left of g(x) */
                zag(get_parent(x)), zig(get_parent(x));
            }
        }
    }
    push_flip(x);
}

struct Forest {
    vector<Node *> nodes;
    int size;

    explicit Forest (int size) : size(size) {
        this->nodes.resize(size);
        for (int i = 0; i < this->size; ++i) {
            this->nodes[i] = new Node(i);
        }
    }

    ~Forest () {
        free(this);
    }
};

void expose (Node *v) {
    splay(v);

    if (v->r_son) {
        v->r_son->path_parent = v;
        v->r_son->parent = nullptr;
        v->r_son = nullptr;
    }

    while (v->path_parent) {
        Node *curr_path_parent = v->path_parent;
        splay(curr_path_parent);

        if (curr_path_parent->r_son) {
            curr_path_parent->r_son->path_parent = curr_path_parent;
            curr_path_parent->r_son->parent = nullptr;
        }

        curr_path_parent->r_son = v;
        v->parent = curr_path_parent;
        v->path_parent = nullptr;

        splay(v);
    }
}

/*** just flip ***/
void make_root (Node *new_root) {
    expose(new_root);
    new_root->fliped = !new_root->fliped;
}

Node *get_most_right_node (Node *v) {
    push_flip(v);
    while (v->r_son) {
        v = v->r_son;
        push_flip(v);
    }
    return v;
}

void cut (Forest *tree, int num_v_1, int num_v_2) {
    if (num_v_1 == num_v_2) {
        return;
    }

    Node *v_1 = tree->nodes[num_v_1], *v_2 = tree->nodes[num_v_2];
    expose(v_1);
    if (v_1->l_son && get_most_right_node(v_1->l_son) == v_2) { /// v_2 is a parent of v_1
        v_1->l_son->parent = nullptr;
        v_1->l_son = nullptr;
    } else { /// Splay to make v_2 have a path_parent (if any exist)
        splay(v_2);
        /// w is a child of v
        if (v_2->l_son == nullptr && v_2->path_parent == v_1) {
            v_2->path_parent = nullptr;
        }
    }
}

void link (Forest *tree, int num_v_1, int num_v_2) {
    if (num_v_1 == num_v_2) {
        return;
    }
    Node *v_1 = tree->nodes[num_v_1], *v_2 = tree->nodes[num_v_2];
    /*** Make v the root of the represented tree (to make the link possible) ***/
    make_root(v_1);
    expose(v_2);

    if (!v_1->parent && !v_1->path_parent) {
        v_2->r_son = v_1;
        v_1->parent = v_2;
    }
}

Node *find_root (Node *v) {
    expose(v);
    while (v->l_son) {
        v = v->l_son;
    }
    splay(v);
    return v;
}

bool connected (Forest *tree, int num_v_1, int num_v_2) {
    if (num_v_1 == num_v_2) {
        return true;
    }

    Node *v_1 = tree->nodes[num_v_1], *v_2 = tree->nodes[num_v_2];
    make_root(v_1);
    expose(v_2);
    return (v_1->parent || v_1->path_parent);
}

int main () {
    string str_query;
    int num_v_1, num_v_2, n, m;
    cin >> n;
    auto *tree = new Forest(n);

    cin >> m;
    for (int i = 0; i < m; ++i) {
        cin >> str_query >> num_v_1 >> num_v_2;
        --num_v_1, --num_v_2;
        if (str_query == "link") {
            link(tree, num_v_1, num_v_2);
        }
        if (str_query == "cut") {
            cut(tree, num_v_1, num_v_2);
        }
        if (str_query == "connected") {
            cout << connected(tree, num_v_1, num_v_2) << '\n';
        }
    }
    return 0;
}