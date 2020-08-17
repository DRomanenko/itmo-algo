#include <iostream>
#include <vector>

using namespace std;

vector<long long> t;
vector<long long> arr;

void build() {
    for (int i = 1; i <= t.size(); ++i) {
        int j = i + (i & -i);
        if (j <= t.size()) {
            t[j] += t[i];
        }
    }
}

void add(int i, long long x) {
    int j = i;
    while (j < t.size() - 1) {
        t[j + 1] += x;
        j = j | (j + 1);
    }
}

long long sum(long long i) {
    long long sum = 0;
    i--;
    while (i >= 0) {
        sum += t[i + 1];
        i = (i & (i + 1)) - 1;
    }
    return sum;
}

void set(int i, long long x) {
    long long d = x - arr[i];
    arr[i] = x;
    add(i - 1, d);
}

int main() {
    iostream::sync_with_stdio(0), cin.tie(0);
    unsigned int n;
    cin >> n;

    t.resize(n + 10);
    arr.resize(n + 10);
    for (int i = 1; i <= n; ++i) {
        long long a;
        cin >> a;
        arr[i] = a;
        t[i] = a;
    }
    build();

    /*for (int i = 0; i <= n; ++i) {
        cout << t[i] << " ";
    }*/

    string s;
    while(cin >> s) {
        if (s[1] == 'u') {
            int i, j;
            cin >> i >> j;
            cout << sum(j) - sum(i - 1) << "\n";
        }
        if (s[1] == 'e') {
            int i;
            long long x;
            cin >> i >> x;
            set(i, x);
        }
    }

    return 0;
}