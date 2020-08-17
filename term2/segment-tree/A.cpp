#include <iostream>

using namespace std;


int main() {
    //#define int long
    iostream::sync_with_stdio(0), cin.tie(0);
    long long  n, x, y, startA;
    cin >> n >> x >> y >> startA;

    long long a[n];
    a[0] = startA;
    for (int i = 1; i < n; ++i) {
        long long save = x * a[i - 1] + y;
        a[i] = (save < 0) ? save + (1 << 16): save % (1 << 16);
    }

    // PREFIX SUM
    for (int i = 1; i < n; ++i) {
        a[i] += a[i - 1];
    }

    long long  m, z, t, startB;
    cin >> m >> z >> t >> startB;
    m = 2 * m;
    long b[m];
    b[0] = startB;
    for (int i = 1; i < m; ++i) {
        b[i] = (z * b[i - 1] + t) % (1 << 30);
    }

    for (int i = 0; i < m; ++i) {
        b[i] = b[i] < 0 ? 0 : b[i] % n;
    }

    long long sum = 0;
    for (int i = 0; i < m / 2; ++i) {
        long long l = min(b[2 * i], b[2 * i + 1]), r = max(b[2 * i], b[2 * i + 1]);
        sum += (l > 0) ? a[r] - a[l - 1] : a[r];
    }

    cout << sum;

    return 0;
}