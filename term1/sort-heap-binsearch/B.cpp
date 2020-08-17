#include <bits/stdc++.h>

typedef long long ll;
typedef long double ld;

using namespace std;

int main(void){
    iostream::sync_with_stdio(0), cin.tie(0);

    int curr;
    vector<int> count(101, 0);

    while (cin >> curr) {
        count[curr]++;
    }

    for (int i = 0; i < 101; ++i) {
        for (int q = count[i]; q--; ) {
            cout << i << ' ';
        }
    }

    cout << "\n";

    return 0;
}