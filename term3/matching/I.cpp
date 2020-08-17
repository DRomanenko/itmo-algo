/** Графы де Брюина **/
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
 
using namespace std;
 
vector<vector<size_t>> graph;
vector<size_t> result;
 
void findEuler(size_t v) {
    while (!graph[v].empty()) {
        size_t u = graph[v].back();
        graph[v].pop_back();
        findEuler(u);
    }
    result.push_back(v);
}
 
int main() {
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    size_t d, k;
    cin >> d >> k;
    if (k == 1) {
        for (size_t num = 0; num < d; ++num)
            cout << num;
        return 0;
    }
    size_t num = (size_t)pow(d, k - 1), lastDigit = num / d;
    graph.resize(num);
    for (size_t i = 0; i < num; ++i) {
        size_t div = (i % lastDigit) * d;
        for (size_t q = 0; q < d; ++q)
            graph[i].push_back(div + q);
    }
    for (size_t i = 0; i < k - 1; ++i)
        cout << 0;
 
    findEuler(0);
    reverse(result.begin(), result.end());
    for (size_t i = 1; i < result.size(); ++i)
        cout << result[i] % d;
    return 0;
}