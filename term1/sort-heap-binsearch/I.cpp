#include <bits/stdc++.h>

typedef long long ll;
typedef long double ld;

using namespace std;

ld BinarySearch (ld key, ld eps) {
    ld left = 0;
    ld right = max((ld)1, key);
    while ((right - left) / 2 > eps) {
        ld middle = left + ((right - left) / 2);
        if (middle * middle + sqrt(middle) < key) {
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
    ld x;

    cin >> x;

    cout << setprecision(7) << BinarySearch(x, 1e-7) << "\n";
    return 0;
}