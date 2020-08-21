/** Алгоритм Куна нахождения наибольшего паросочетания **/
#include <iostream>
#include <vector>
 
using namespace std;
 
vector<vector<int>> verReachableFromLeftPart;
vector<bool> visited;
vector<int> takenInMatching;
 
bool findAddPath(size_t v) {
    if (visited[v])
        return false;
 
    visited[v] = true;
    for (const auto &to : verReachableFromLeftPart[v]) {
        if (takenInMatching[to] == -1 || findAddPath(takenInMatching[to])) {
            takenInMatching[to] = v;
            return true;
        }
    }
    return false;
}
 
int main() {
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    size_t numLeft, numRight, numEdges;
    cin >> numLeft >> numRight >> numEdges;
 
    verReachableFromLeftPart.resize(numLeft);
    for (size_t i = 0; i < numEdges; ++i) {
        int v, u;
        cin >> v >> u;
        verReachableFromLeftPart[v - 1].push_back(u - 1);
    }
 
    visited.assign(numLeft, false);
    takenInMatching.assign(numRight, -1);
    for (size_t v = 0; v < numLeft; ++v)
        if (findAddPath(v))
            visited.assign(numLeft, false);
 
 
    int numVertexInMatching = 0;
    for (const auto &v : takenInMatching)
        if (v != -1)
            ++numVertexInMatching;
    cout << numVertexInMatching;
}