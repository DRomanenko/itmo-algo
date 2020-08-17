/** Нахождение Эйлерова пути за O (E) **/
#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>
 
using namespace std;
 
vector<vector<int>> matrix;
 
int main() {
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    size_t numVer;
    cin >> numVer;
    matrix.assign(numVer, vector<int>(numVer, 0));
    int numOddlyVer = 0, first = -1;
    for (size_t i = 0; i < numVer; ++i) {
        size_t qtyVer;
        cin >> qtyVer;
 
        if (qtyVer % 2) {
            first = i;
            ++numOddlyVer;
        }
 
        for (size_t q = 0; q < qtyVer; ++q) {
            size_t v;
            cin >> v;
            ++matrix[i][v - 1];
        }
    }
    if (numOddlyVer > 2) {
        cout << -1;
        return 0;
    }
 
    first = numOddlyVer ? first : 0;
    stack<int> st;
    st.push(first);
    vector<int> result;
    while (!st.empty()) {
        int v = st.top();
        size_t to = 0;
        for (to = 0; to < numVer; ++to)
            if (matrix[v][to])
                break;
        if (to == numVer) {
            st.pop();
            result.push_back(v);
        } else {
            --matrix[v][to];
            --matrix[to][v];
            st.push(to);
        }
    }
 
    cout << result.size() - 1 << '\n';
    reverse(result.begin(), result.end());
    for (const auto &v : result)
        cout << v + 1 << ' ';
    return 0;
}