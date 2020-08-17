/** Связь максимального паросочетания и минимального вершинного покрытия в двудольных графах **/
#include <iostream>
#include <vector>
#include <algorithm>
 
using namespace std;
 
vector<vector<bool>> verReachableFromLeftPart;
vector<vector<bool>> verReachableFromRightPart;
vector<bool> visitedLeftPart;
vector<bool> visitedRightPart;
vector<int> takenInMatchingInLeftPart;
 
void dfs(int v, bool isFromLeftPart) {
    vector<vector<bool>> &verReachable = (isFromLeftPart ?
                                          verReachableFromLeftPart : verReachableFromRightPart);
    vector<bool> &visited = (isFromLeftPart ? visitedLeftPart : visitedRightPart);
    if (visited[v])
        return;
    visited[v] = true;
    for (int to = 0; (size_t) to < verReachable[v].size(); ++to) {
        if (verReachable[v][to]) {
            if (isFromLeftPart ? v != takenInMatchingInLeftPart[to] :
                to == takenInMatchingInLeftPart[v])
                dfs(to, !isFromLeftPart);
        }
    }
}
 
vector<bool> visited;
 
bool findAddPath(size_t v) {
    if (visited[v])
        return false;
 
    visited[v] = true;
    for (size_t to = 0; to < verReachableFromLeftPart[v].size(); ++to) {
        if (verReachableFromLeftPart[v][to]) {
            if (takenInMatchingInLeftPart[to] == -1 || findAddPath(takenInMatchingInLeftPart[to])) {
                takenInMatchingInLeftPart[to] = v;
                return true;
            }
        }
    }
    return false;
}
 
int main() {
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    size_t k;
    cin >> k;
    for (size_t t = 0; t < k; ++t) {
        size_t numLeft, numRight;
        cin >> numLeft >> numRight;
        verReachableFromLeftPart.assign(numLeft, vector<bool>(numRight, true));
        verReachableFromRightPart.assign(numRight, vector<bool>(numLeft, true));
        for (size_t v = 0; v < numLeft; ++v) {
            int to;
            while (cin >> to && to != 0) {
                verReachableFromLeftPart[v][to - 1] = false;
                verReachableFromRightPart[to - 1][v] = false;
            }
        }
 
        visited.assign(numLeft, false);
        takenInMatchingInLeftPart.assign(numRight, -1);
        for (size_t v = 0; v < numLeft; ++v)
            if (findAddPath(v))
                visited.assign(numLeft, false);
 
        vector<bool> inMatching(numLeft, false);
        for (size_t v = 0; v < numRight; ++v)
            if (takenInMatchingInLeftPart[v] != -1)
                inMatching[takenInMatchingInLeftPart[v]] = true;
 
        visitedLeftPart.assign(numLeft, false);
        visitedRightPart.assign(numRight, false);
        for (size_t v = 0; v < numLeft; ++v)
            if (!visitedLeftPart[v] && !inMatching[v])
                dfs(v, true);
 
        size_t numVerLeft = count(visitedLeftPart.begin(), visitedLeftPart.end(), true),
                numVerRight = count(visitedRightPart.begin(), visitedRightPart.end(), false);
        cout << numVerLeft + numVerRight << '\n';
        cout << numVerLeft << ' ' << numVerRight << ' ' << '\n';
        for (size_t v = 0; v < numLeft; ++v)
            if (visitedLeftPart[v])
                cout << v + 1 << ' ';
        cout << '\n';
        for (size_t v = 0; v < numRight; ++v)
            if (!visitedRightPart[v])
                cout << v + 1 << ' ';
        cout << "\n\n";
    }
    return 0;
}