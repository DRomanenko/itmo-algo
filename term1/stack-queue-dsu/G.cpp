#include <bits/stdc++.h>

using namespace std;

struct node {
    int value, Min, Max, Size;
    node(int new_value = 0) {
        value = new_value;
        Min = new_value;
        Max = new_value;
        Size = 1;
    }
};

vector<node*> Rank, parents;

void make_v (node* v) {
    parents[v -> value] = v;
    Rank[v -> value] = 0;
}

node* get (node* v) {
    if (parents[v -> value] -> value != v -> value) {
        parents[v -> value] = get(parents[v -> value]);
    }
    node* boi = parents[v -> value];
    node* boii = v;
    return parents[v -> value];
}

void join (node* x, node* y) {
    x = get(x);
    y = get(y);
    if (x != y) {
        if (Rank[x -> value] > Rank[y -> value]) {
            swap(x, y);
        }
        y -> Min = min(y -> Min, x -> Min);
        y -> Max = max(y -> Max, x -> Max);
        y -> Size += x -> Size;
        parents[y -> value] = y;
        parents[x -> value] = y;
        if (Rank[x -> value] == Rank[y -> value]) {
            Rank[y -> value]++;
        }
    }
}

int main() {
    iostream::sync_with_stdio(false), cin.tie(nullptr);
    unsigned int n;
    cin >> n;
    Rank.resize(n + 1);
    parents.resize(n + 1);
    for (int i = 0; i < n; ++i) {
        node* X = new node(i + 1);
        make_v(X);
    }
    /*
    for (auto v: parents) {
        if (v != nullptr)
            cout << v -> Min << " " << v -> Max << " " << v -> Size << endl;
    }
    */
    string s;
    while (cin >> s) {
        if (s == "union") {
            int x, y;
            cin >> x >> y;
            node* X = new node(x);
            node* Y = new node(y);
            join(X, Y);
        }
        if (s == "get") {
            int x;
            cin >> x;
            node* v = new node(x);
            v = get(v);
            cout << v -> Min << " " << v -> Max << " " << v -> Size << endl;
        }
    }
    return 0;
}