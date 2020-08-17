#include <bits/stdc++.h>

typedef long long ll;
typedef long double ld;

using namespace std;

ld p, f, a;

ld functionR (ld x) {
    return sqrt((1 - a) * (1 - a) + x * x) / p + sqrt(a * a + (1 - x) * (1 - x)) / f;
}

ld ternarySearch (ld eps) {
    ld left = 0;
    ld right = 1;
    while ((right - left) / 2 > eps) {
        ld middleLeft = left + ((right - left) / 3);
        ld middleRight = right - ((right - left) / 3);
        if (functionR(middleLeft) >= functionR(middleRight)) {
            left = middleLeft;
        }
        else {
            right = middleRight;
        }
    }
    return left;
}

int main(void){
    iostream::sync_with_stdio(0), cin.tie(0);

    cin >> p >> f >> a;

    cout << setprecision(5) << ternarySearch(1e-7) << "\n";
    return 0;
}