#include <bits/stdc++.h>

using namespace std;

#define ll long long
const ll MAX = static_cast<const ll>(1E15);

ll bit (ll i, ll mask) {
    return ((mask >> i) & 1);
}

ll count (ll mask) {
    ll x = mask, result = 0;
    while (x != 0) {
        result += x & 1;
        x >>= 1;
    }
    return result;
}

int main () {
    iostream::sync_with_stdio(false), cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<ll>> length(n, vector<ll>(n));

    for (int i = 0; i < n; ++i) {
        for (int q = 0; q < n; ++q) {
            cin >> length[i][q];
        }
    }

    /*for (int i = 0; i < (1 << n); ++i) {
        for (int q = n; q >= 0; --q) {
            cout << ((i >> q) & 1);
        }
        cout << endl;
    }*/

    vector<vector<ll>> d((1 << n), vector<ll>(n));
    /*  d[mask][i] - длина кратчайшего гамильтонова пути подмножества
     *  вершин mask, заканчивающегося в вершине i
     */

    //Переход
    for (int mask = 0; mask < (1 << n); ++mask) {
        for (int i = 0; i < n; ++i) {
            if (count(mask) == 1 && bit(i, mask) == 1) {
                d[mask][i] = 0;
            } else if (count(mask) > 1 && bit(i, mask) == 1) {
                ll save = MAX;
                for (int q = 0; q < n; ++q) {
                    if (bit(q, mask) == 1 && i != q) {
                        save = min(save, d[mask ^ (1 << i)][q] + length[q][i]);
                    }
                }
                d[mask][i] = save;
            } else {
                d[mask][i] = MAX;
            }
        }
    }
    ll ans = MAX, save = 0;
    for (int i = 0; i < n; ++i) {
        if (d[(1 << n) - 1][i] <= ans) {
            ans = d[(1 << n) - 1][i];
            save = i;
        }
    }
    cout << ans << endl;

    // Восстановление ответа
    vector<int> way;
    way.push_back(save);
    int i = save, mask = (1 << n) - 1;
    while (count(mask) != 1) {
        for (int q = 0; q < n; ++q) {
            if (d[mask][i] == d[mask ^ (1 << i)][q] + length[q][i]) {
                mask &= ~(1 << i);
                way.push_back(q);
                i = q;
                break;
            }
        }
    }
    for (i = 0; i < way.size(); ++i) {
        cout << way[i] + 1 << " ";
    }
    /*
    for (int mask = 0; mask < (1 << n); ++mask) {
        for (int i = 0; i < n; ++i) {
            if (d[mask][i] == MAX) {
                cout << -1 << ' ';
            } else {
                cout << d[mask][i] << ' ';
            }
        }
        cout << endl;
    }*/

    return 0;
}