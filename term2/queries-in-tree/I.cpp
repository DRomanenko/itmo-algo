#include <iostream>
#include <vector>
#include <map>

using namespace std;

template <class T>
struct Node {
    T key;
    long long size, priority;
    Node<T> *l_son = nullptr, *r_son = nullptr, *parent = nullptr;

    explicit Node (T _v) : key(_v), size(1), priority((rand() << 15) | rand()) {
    }

    static long long get_size (Node<T> *v) {
        return v ? v->size : 0;
    }

    void upd_parent () {
        if (!this) { return; }
        if (this->l_son) { this->l_son->parent = this; }
        if (this->r_son) { this->r_son->parent = this; }
    }

    Node<T> *update () {
        if (!this) { return this; }
        upd_parent();
        size = 1 + get_size(l_son) + get_size(r_son);
        return this;
    }

    int index () {
        int ind = get_size(this->l_son);
        Node<T> *v = this;
        while (v->parent) {
            if (v->parent->l_son != v) {
                ind += get_size(v->parent->l_son) + 1;
            }
            v = v->parent;
        }
        return ind;
    }

    Node<T> *root () {
        return this->parent ? parent->root() : this;
    }

    Node<T> *leftest () {
        return this->l_son ? l_son->leftest() : this;
    }

    Node<T> *rightest () {
        return this->r_son ? r_son->rightest() : this;
    }
};

template <class T>
Node<T> *merge (Node<T> *root_1, Node<T> *root_2) {
    if (root_1 == nullptr) { return root_2; }
    if (root_2 == nullptr) { return root_1; }

    if (root_1->priority > root_2->priority) {
        root_1->r_son = merge(root_1->r_son, root_2);
        return root_1->update();
    } else {
        root_2->l_son = merge(root_1, root_2->l_son);
        return root_2->update();
    }
}

template <class T>
pair<Node<T> *, Node<T> *> split_index (Node<T> *root, int index) {
    if (root == nullptr) { return {nullptr, nullptr}; }

    root->parent = nullptr;
    if (index <= Node<T>::get_size(root->l_son)) {
        auto help = split_index(root->l_son, index);
        root->l_son = help.second;
        return {help.first, root->update()};
    } else {
        auto help = split_index(root->r_son, index - Node<T>::get_size(root->l_son) - 1);
        root->r_son = help.first;
        return {root->update(), help.second};
    }
}

template <class T>
pair<Node<T> *, Node<T> *> split_node (Node<T> *root) {
    if (!root) return {nullptr, nullptr};

    Node<T> *after_root = root, *help = root;
    root = root->l_son;
    if (root) {
        root->parent = help->l_son = nullptr;
    } else {
        while (help->parent && help->parent->l_son == after_root) {
            help = after_root = help->parent;
        }
        if (help->parent) {
            help = root = help->parent, help->r_son = after_root->parent = nullptr;
        }
    }

    while (help->update()->parent) {
        if (help->parent->l_son == help) {
            if (help == root) {
                swap(root->parent, after_root->parent);
                after_root->parent->l_son = after_root;
            }
            help = after_root = after_root->parent;
        } else {
            if (help == after_root) {
                swap(root->parent, after_root->parent);
                root->parent->r_son = root;
            }
            help = root = root->parent;
        }
    }
    return {root, after_root};
}

template <class T>
class EulerTourTree {
public:
    explicit EulerTourTree (T n) {
        nodes.reserve(n), edges.reserve(n);
        for (T i = 0; i < n; ++i) {
            add_vertex();
        }
    }

    T add_vertex () {
        T id = nodes.size();
        nodes.emplace_back(edge{id, id});
        edges.emplace_back();
        return id;
    }

    void attach (T v_1, T v_2) {
        auto help = split_node(&nodes[v_2]);
        (void) merge(help.first,
                     merge(&(edges[v_2].emplace(v_1, Node<edge>{edge{v_2, v_1}}).first)->second,
                           merge(nodes[v_1].root(),
                                 merge(&(edges[v_1].emplace(v_2, Node<edge>{edge{v_1, v_2}}).first)->second,
                                       help.second))));
    }

    void flip (T v) {
        auto help = split_node(&nodes[v]);
        (void) merge(help.second, help.first);
    }

    void link (T v_1, T v_2) {
        flip(v_1);
        attach(v_1, v_2);
    }

    void cut (T v_1, T v_2) {
        auto edge_v_1_2 = edges[v_1].find(v_2), edge_v_2_1 = edges[v_2].find(v_1);
        if (edge_v_1_2->second.index() > edge_v_2_1->second.index()) {
            swap(v_1, v_2), swap(edge_v_1_2, edge_v_2_1);
        }
        auto help = split_node(&edge_v_1_2->second);
        (void) split_index(help.second, 1);
        (void) merge(help.first, split_index(split_node(&edge_v_2_1->second).second, 1).second);
        edges[v_1].erase(edge_v_1_2), edges[v_2].erase(edge_v_2_1);
    }

    bool connected (T v_1, T v_2) {
        return nodes[v_1].root() == nodes[v_2].root();
    }

    T get_size (T v) {
        return (nodes[v].root()->size + 2) / 3;
    }

private:
    struct edge {
        T u, v;
    };
    vector<Node<edge>> nodes; /// loop in yourself
    vector<map<T, Node<edge>>> edges;
};

int main () {
    iostream::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    string str_query;
    int num_v_1, num_v_2, n, m;
    cin >> n;
    auto *tree = new EulerTourTree<long long>(++n);

    cin >> m;
    for (int i = 0; i < m; ++i) {
        cin >> str_query;
        if (str_query == "link") {
            cin >> num_v_1 >> num_v_2;
            tree->link(--num_v_1, --num_v_2);
        }
        if (str_query == "cut") {
            cin >> num_v_1 >> num_v_2;
            tree->cut(--num_v_1, --num_v_2);
        }
        if (str_query == "size") {
            cin >> num_v_1;
            cout << tree->get_size(--num_v_1) << '\n';
        }
    }
    return 0;
}