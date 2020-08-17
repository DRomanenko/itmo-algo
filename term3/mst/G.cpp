#include <iostream>
#include <vector>
#include <cmath>
 
using namespace std;
 
const size_t MAXS = 5000;
 
vector<pair<int, int>> point;
vector<bool> used(MAXS, false);
vector<long double> dist(MAXS, (long double) INT32_MAX);
 
long double prim(size_t n) {
    dist[0] = 0;
    long double ans = 0;
    for (size_t i = 0; i < n; ++i) {
        int v = -1;
        for (size_t to = 0; to < n; ++to)
            if (!used[to] && (v == -1 || dist[to] < dist[v]))
                v = to;
 
        used[v] = true;
        ans += dist[v];
 
        for (size_t to = 0; to < n; ++to) {
            long double curr_dist = sqrt(abs(point[to].first - point[v].first)
                                    * abs(point[to].first - point[v].first)
                                    + abs(point[to].second - point[v].second)
                                      * abs(point[to].second - point[v].second));
            if (curr_dist < dist[to])
                dist[to] = curr_dist;
        }
    }
    return ans;
}
 
int main() {
    /* freopen("truth.in", "rank", stdin);
    freopen("truth.out", "w", stdout); */
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    size_t n;
    cin >> n;
 
    for (size_t i = 0; i < n; ++i) {
        int x, y;
        cin >> x >> y;
        point.emplace_back(x, y);
    }
 
    cout.precision(12);
    cout << prim(n) << '\n';
    return 0;
}