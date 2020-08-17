#include <bits/stdc++.h>

using namespace std;

int sum = 0;

struct node {
    int value;

    explicit node(int new_value = 0) {
        value = new_value;
    }
};

vector<node*> parents;
vector<int> Rank;
vector<int> experience;

void make_v (node* v) {
    parents[v -> value] = v;
    Rank[v -> value] = 0;
    experience[v -> value] = 0;
}

node* get (node* v) {
    sum += experience[v -> value];
    if (parents[v -> value] -> value == v -> value) {
        return v;
    }
    return get(parents[v -> value]);
}

void join (node* x, node* y) {
    x = get(x);
    y = get(y);
    if (x -> value != y -> value) {
        if (Rank[x -> value] > Rank[y -> value]) {
            swap(x, y);
        }
        experience[x -> value] -= experience[y -> value];
        parents[x -> value] = y;
        if (Rank[x -> value] == Rank[y -> value]) {
            Rank[y -> value]++;
        }
    }
}

void add (node* v, int x) {
    experience[get(v) -> value] += x;
}

int main() {
    iostream::sync_with_stdio(false), cin.tie(nullptr);
    unsigned int n, m;
    cin >> n >> m;
    Rank.resize(n + 2);
    parents.resize(n + 2);
    experience.resize(n + 2);
    for (int i = 0; i <= n; ++i) {
        node* X = new node(i + 1);
        make_v(X);
    }

    for (int i = 0; i < m; ++i) {
        string s;
        cin >> s;
        if (s == "join") {
            int x, y;
            cin >> x >> y;
            node* X = new node(x);
            node* Y = new node(y);
            join(X, Y);
        }
        if (s == "add") {
            int x, y;
            cin >> x >> y;
            node* v = new node(x);
            add(v, y);
        }
        if (s == "get") {
            int x;
            cin >> x;
            node* v = new node(x);
            sum = 0;
            get(v);
            cout << sum << endl;
        }
        // cout << experience[1] << " " << experience[2] << " " << experience[3] << " " << experience[3] << endl;
    }
    return 0;
}