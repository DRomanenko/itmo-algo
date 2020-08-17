#include <bits/stdc++.h>

typedef long long ll;
typedef long double ld;

using namespace std;

ll BinarySearch (ll key, ll w, ll h) {
    ll left = 0;
    ll right = key * (w + h);
    while (right > left + 1) {
        ll middle = left + ((right - left) >> 1);
        if ((middle / w) * (middle / h) < key) {
            left = middle;
        }
        else {
            right = middle;
        }
    }
    return right;
}

int main(void){
    iostream::sync_with_stdio(0), cin.tie(0);
    ll n, w, h;

    cin >> w >> h >> n;

    cout << BinarySearch(n, w, h) << "\n";
    return 0;
}