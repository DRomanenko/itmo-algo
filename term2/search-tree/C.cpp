#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>

using namespace std;

struct node {
    long key, priority, index;
    node *left_son, *right_son, *parent;

    node (long long new_key, long long new_priority, long long new_index) : key(new_key), priority(new_priority),
                                                                            index(new_index),
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

bool comparator_for_key (node *a, node *b) {
    return (a -> key < b -> key);
}

bool comparator_for_index (node *a, node *b) {
    return (a -> index < b -> index);
}

int main () {
    //freopen("markchain.in", "r", stdin);
    //freopen("markchain.out", "w", stdout);
    iostream::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int n, a, b;
    cin >> n;

    vector<node *> nodes;

    stack<node *> st;

    for (int i = 0; i < n; ++i) {
        cin >> a >> b;
        nodes.push_back(new node(a, -b, i)); /// т.к по минимуму куча
    }

    /** Декартово дерево можно построить всегда **/
    cout << "YES\n";

    sort(nodes.begin(), nodes.end(), comparator_for_key);
    /*for (int i = 0; i < n; ++i) {
        cout << nodes[i] -> key << ':' << nodes[i] -> priority << ' ';
    }
    cout << '\n';*/

    node *prev_node = nullptr;
    /** Построение за O(n) **/
    for (auto curr_node : nodes) {
        /** Поднимаемся по правой ветке **/
        while (!st.empty() && st.top() -> priority <= curr_node -> priority) {
            prev_node = st.top();
            st.pop();
        }

        ///      (1)                           (2)
        ///   prev_node        --->        curr_node
        ///        \                          /
        ///     curr_node    <---        prev_node

        /** (1) **/
        if (prev_node != nullptr) {
            prev_node -> parent = curr_node;
            curr_node -> left_son = prev_node;
        }

        /** (2) **/
        if (!st.empty()) {
            curr_node -> parent = st.top();
            st.top() -> right_son = curr_node;
        }
        st.push(curr_node);
        prev_node = nullptr;
    }

    sort(nodes.begin(), nodes.end(), comparator_for_index);
    for (auto curr_node : nodes) {
        cout << (curr_node -> parent == nullptr ? 0 : curr_node -> parent -> index + 1) << ' ';
        cout << (curr_node -> left_son == nullptr ? 0 : curr_node -> left_son -> index + 1) << ' ';
        cout << (curr_node -> right_son == nullptr ? 0 : curr_node -> right_son -> index + 1) << '\n';
    }
    return 0;
}