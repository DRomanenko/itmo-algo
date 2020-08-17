#include <bits/stdc++.h>

typedef long long ll;
typedef long double ld;

using namespace std;

ll countt = 0;
vector<ll> help;

void Merge (vector<ll> &a, ll left, ll middle, ll right) {
    ll i = left, j = middle, k = left;

    while (i < middle && j < right + 1) {
        if (a[i] <= a[j]) {
            help[k++] = a[i++];
        } else {
            help[k++] = a[j++];
            countt += (middle - i);
        }
    }

    while (i < middle) {
        help[k++] = a[i++];
    }
    while (j < right + 1) {
        help[k++] = a[j++];
    }

    for (i = left; i < right + 1; ++i) {
        a[i] = help[i];
    }
}

void MergeSort (vector<ll> &a, ll left, ll right) {
    if(left >= right) {
        return;
    }
    ll middle = left + ((right - left) >> 1);
    MergeSort(a, left, middle);
    MergeSort(a, middle + 1, right);
    Merge(a, left, middle + 1, right);
}

int main(void){
    iostream::sync_with_stdio(0), cin.tie(0);
    ll n;

    cin >> n;

    vector<ll> array(n);
    help.resize(n);

    for (ll i = 0; i < n; ++i) {
        cin >> array[i];
    }

    MergeSort (array, 0, n - 1);

    cout << countt << "\n";
    return 0;
}