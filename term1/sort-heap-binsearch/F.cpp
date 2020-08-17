#include <bits/stdc++.h>

typedef long long ll;
typedef long double ld;

using namespace std;

ll BinarySearch (vector<ll> &a, ll key) {
    ll left = 0;
    ll right = a.size() - 1;
    while (right > left + 1) {
        ll middle = left + ((right - left) >> 1);
        if (a[middle] <= key) {
            left = middle;
        }
        else {
            right = middle;
        }
    }
    if (abs(a[right] - key) < abs(a[left] - key)) {
        return a[right];
    } else {
        return a[left];
    }
}

int main(void){
    iostream::sync_with_stdio(0), cin.tie(0);

    ll n, k;
    cin >> n >> k;

    vector<ll> array(n);

    for (ll i = 0; i < n; ++i) {
        cin >> array[i];
    }

    for (ll i = 0; i < k; ++i) {
        ll curr;
        cin >> curr;
        cout << BinarySearch(array, curr)<< "\n";
    }

    return 0;
}