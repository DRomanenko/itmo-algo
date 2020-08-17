#include <bits/stdc++.h>

typedef long long ll;
typedef long double ld;

using namespace std;

int n, k;
vector<pair<pair<ld, ld>, int> > v;

ld middle;

bool cmp (pair<pair<ld, ld>, int> a, pair<pair<ld, ld>, int> b) {
    return a.first.first - middle * a.first.second > b.first.first - middle * b.first.second;
}

bool f () {
    ld suma = 0, sumb = 0;
    sort(v.begin(), v.end(), cmp);
    for (int i = 0; i < k; ++i) {
        suma += v[i].first.first;
        sumb += v[i].first.second;
    }
    return suma >= middle * sumb;
}

ld BinarySearch () {
    ld left = 0.0,
        right = 2e+11;
    unsigned int cikada = 60;
    while (cikada--) {
        middle = left + (right - left) / 2;
        if (f()) {
            left = middle;
        }
        else {
            right = middle;
        }
    }
    return left;
}


int main(void) {
    iostream::sync_with_stdio(0), cin.tie(0);
    ifstream cin("kbest.in");
    ofstream cout("kbest.out");

    cin >> n >> k;
    v.resize(n);

    for (int i = 0; i < n; ++i) {
        int a, b;
        cin >> a >> b;
        v[i] = {{a, b}, i};
    }

    //cout <<
    BinarySearch();
   // << '\n';

    for (int i = 0; i < k; ++i) {
        cout << v[i].second + 1 << "\n";
    }

    return 0;
}