#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

unsigned int n;

vector<vector<long long>> sparse_table;

long long op_min (long long a, long long b) {
    return a > b ? b : a;
}

void sparse_build () {
    for (int i = 1; (1 << i) <= n; ++i) {
        sparse_table.resize(sparse_table.size() + 1);
        for (int j = 0; j <= n - (1 << i); ++j) {
            sparse_table[i].push_back(op_min(sparse_table[i - 1][j], sparse_table[i - 1][j + (1 << (i - 1))]));
        }
    }
}

long long find_min (int l, int r) {
    int length = r - l + 1, level = (int) log2(length);
    return op_min(sparse_table[level][l], sparse_table[level][r - (1 << level) + 1]);
}

struct Query {
    int left = 0;
    int right = 0;
    long long ans = 0;
};

vector<Query> a;

bool is_empty_a () {
    return a.empty();
}

void sift_up_a (int x) {
    int i = x;
    while (i > 0 && a[i].left < a[(i - 1) / 2].left) {
        swap(a[i], a[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void sift_down_a (int i) {
    int min_index = i;
    if (2 * i + 1 < (int) a.size() && a[2 * i + 1].left < a[min_index].left) {
        min_index = 2 * i + 1;
    }
    if (2 * i + 2 < (int) a.size() && a[2 * i + 2].left < a[min_index].left) {
        min_index = 2 * i + 2;
    }
    if (min_index != i) {
        swap(a[i], a[min_index]);
        sift_down_a(min_index);
    }
}

void push_a (Query x) {
    a.push_back(x);
    sift_up_a(a.size() - 1);
}

Query extract_min_a () {
    Query ans = a[0];
    a[0] = a.back();
    a.pop_back();
    sift_down_a(0);
    return ans;
}

Query min_element_a () {
    return a[0];
}

vector<Query> b;

bool is_empty_b () {
    return b.empty();
}

void sift_up_b (int x) {
    int i = x;
    while (i > 0 && b[i].ans > b[(i - 1) / 2].ans) {
        swap(b[i], b[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void sift_down_b (int i) {
    int min_index = i;
    if (2 * i + 1 < (int) b.size() && b[2 * i + 1].ans > b[min_index].ans) {
        min_index = 2 * i + 1;
    }
    if (2 * i + 2 < (int) b.size() && b[2 * i + 2].ans > b[min_index].ans) {
        min_index = 2 * i + 2;
    }
    if (min_index != i) {
        swap(b[i], b[min_index]);
        sift_down_b(min_index);
    }
}

void push_b (Query x) {
    b.push_back(x);
    sift_up_b(b.size() - 1);
}

Query extract_min_b () {
    Query ans = b[0];
    b[0] = b.back();
    b.pop_back();
    sift_down_b(0);
    return ans;
}

Query min_element_b () {
    return b[0];
}

int main () {
    iostream::sync_with_stdio(false), cin.tie(nullptr);
    freopen("rmq.in", "r", stdin);
    freopen("rmq.out", "w", stdout);
    int m;
    cin >> n >> m;

    vector<Query> query(m);

    for (int i = 0; i < m; ++i) {
        int l, r;
        long long new_border;
        cin >> l >> r >> new_border;
        query[i].left = l - 1;
        query[i].right = r - 1;
        query[i].ans = new_border;
        push_a(query[i]);
    }


    vector<long long> arr;
    for (int i = 0; i < n; ++i) {
        while (!is_empty_a()) {
            Query curr = min_element_a();
            if (curr.left > i) {
                break;
            }

            push_b(curr);
            extract_min_a();
        }

        while (!is_empty_b() && min_element_b().right < i) {
            extract_min_b();
        }

        if (is_empty_b()) {
            arr.push_back(0);
        } else {
            arr.push_back(min_element_b().ans);
        }
    }
    /*for (auto u : arr) {
        cout << u << " ";
    }*/

    sparse_table.resize(1);
    sparse_table[0].resize(n);
    for (int i = 0; i < n; ++i) {
        sparse_table[0][i] = arr[i];
    }
    sparse_build();

    for (int i = 0; i < m; ++i) {
        if (find_min(query[i].left, query[i].right) != query[i].ans) {
            cout << "inconsistent";
            return 0;
        }
    }
    cout << "consistent\n";
    for (auto u : arr) {
        cout << u << " ";
    }

    return 0;
}