#include <bits/stdc++.h>

typedef long long ll;
typedef long double ld;

using namespace std;

ll BinarySearch (ll key, ll x, ll y) {
    ll left = 0;
    ll right = key * max (x, y);
    while (right > left + 1) {
        ll middle = left + ((right - left) >> 1);
        if ((middle / x + middle / y) < key) {
            left = middle;
        }
        else {
            right = middle;
        }
    }
    return right + min(x, y);
}

int main(void){
    iostream::sync_with_stdio(0), cin.tie(0);
    ll n, x, y;
    
    cin >> n >> x >> y;
    
    cout << BinarySearch(n - 1, x, y) << "\n";
    return 0;
}