/** Связь максимального паросочетания и минимального вершинного покрытия в двудольных графах **/
#include <iostream>
#include <vector>
#include <algorithm>
 
using namespace std;
 
vector<vector<int>> verReachableFromLeftPart;
vector<vector<int>> verReachableFromRightPart;
vector<bool> visitedLeftPart;
vector<bool> visitedRightPart;
vector<int> takenInMatchingInLeftPart;
vector<int> takenInMatchingInRightPart;
 
void dfs(size_t v, bool isFromLeftPart) {
    vector<vector<int>> &verReachable = (isFromLeftPart ?
                                         verReachableFromLeftPart : verReachableFromRightPart);
    vector<bool> &visited = (isFromLeftPart ? visitedLeftPart : visitedRightPart);
    vector<int> &takenInMatching = (isFromLeftPart ?
                                    takenInMatchingInLeftPart : takenInMatchingInRightPart);
    if (visited[v])
        return;
    visited[v] = true;
    for (const auto &to : verReachable[v])
        if (isFromLeftPart ? to != takenInMatching[v] : to == takenInMatching[v])
            dfs(to, !isFromLeftPart);
}
 
int main() {
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    size_t numLeft, numRight;
    cin >> numLeft >> numRight;
 
    verReachableFromLeftPart.resize(numLeft);
    verReachableFromRightPart.resize(numRight);
    for (size_t v = 0; v < numLeft; ++v) {
        size_t numEdgesFromV;
        cin >> numEdgesFromV;
        for (size_t i = 0; i < numEdgesFromV; ++i) {
            int to;
            cin >> to;
            verReachableFromLeftPart[v].push_back(to - 1);
            verReachableFromRightPart[to - 1].push_back(v);
        }
    }
 
    takenInMatchingInLeftPart.assign(numLeft, -1);
    takenInMatchingInRightPart.assign(numRight, -1);
    for (size_t v = 0; v < numLeft; ++v) {
        int to;
        cin >> to;
        if (to != 0) {
            takenInMatchingInLeftPart[v] = to - 1;
            takenInMatchingInRightPart[to - 1] = v;
        }
    }
 
    visitedLeftPart.assign(numLeft, false);
    visitedRightPart.assign(numRight, false);
    for (size_t v = 0; v < numLeft; ++v)
        if (!visitedLeftPart[v] && takenInMatchingInLeftPart[v] == -1)
            dfs(v, true);
 
    size_t numVerLeft = count(visitedLeftPart.begin(), visitedLeftPart.end(), false),
            numVerRight = count(visitedRightPart.begin(), visitedRightPart.end(), true);
    cout << numVerLeft + numVerRight << '\n';
    cout << numVerLeft << ' ';
    for (size_t v = 0; v < numLeft; ++v)
        if (!visitedLeftPart[v])  // L-
            cout << v + 1 << ' ';
    cout << '\n';
 
    cout << numVerRight << ' ';
    for (size_t v = 0; v < numRight; ++v)
        if (visitedRightPart[v])  // R+
            cout << v + 1 << ' ';
    return 0;
}