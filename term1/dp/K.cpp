#include <bits/stdc++.h>

using namespace std;

#define ll long long
const ll MAX = static_cast<const ll>(1E15);

ll bit (ll i, ll mask) {
    return ((mask >> i) & 1);
}

int main () {
    iostream::sync_with_stdio(false), cin.tie(nullptr);
    ifstream cin("skyscraper.in");
    ofstream cout("skyscraper.out");

    int n, w;
    cin >> n >> w;

    int cost[n];
    for (int i = 0; i < n; ++i) {
        cin >> cost[i];
    }

    vector<pair<ll, ll>> d(1 << n);
    vector<ll> parents(1 << n);

    //База
    d[0] = {0, 0}; // пустое множество 0 кораблей
    //B | A == A - B - подмножество A

    //Переход
    for (int A = 1; A < (1 << n); ++A) {
        d[A] = {MAX, MAX};
        int save = 0;
        for (int x = 0; x < n; ++x) {
            if (bit(x, A)) {
                pair<ll, ll> curr = d[A & (~(1 << x))];
                if (curr.second + cost[x] <= w) {
                    curr.second += cost[x];
                } else {
                    curr.first++;
                    curr.second = cost[x];
                }
                if (d[A] > curr) {
                    d[A] = curr;
                    parents[A] = x;
                }
            }
        }
    }
    ll trueCount = (d[(1 << n) - 1].first + (d[(1 << n) - 1].second > 0));
    cout << trueCount << endl;
    int beg = (1 << n) - 1;
    vector<ll> truePor;
    while (beg > 0) {
        truePor.push_back(parents[beg]);
        beg -= 1 << parents[beg];
    }

    int saver = 0;
    vector<vector<int>> trueParents(trueCount);
    for (int i = 0; i < trueCount; ++i) {
        ll sum = 0;
        for (int q = saver; q < truePor.size(); ++q) {
            sum += cost[truePor[q]];
            if (sum <= w) {
                trueParents[i].push_back(truePor[q]);
            } else {
                saver = q;
                break;
            }
        }
    }
    ll backSum = 0;
    for (auto u : trueParents) {
        cout << u.size();
        backSum += u.size();
        ll sum = 0;
        sort(u.begin(), u.end());
        for (auto y : u) {
            cout << " " << y + 1;
            sum += cost[y];
        }
        //cout << " = " << sum;
        cout << endl;
    }
    return 0;
}