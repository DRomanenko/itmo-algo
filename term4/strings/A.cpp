#include <iostream>
#include <vector>
 
using namespace std;
 
/** Наивная реализация */
void findAllSubStr(vector<size_t> *ans, string &s, string &t) {  // O(t.size() * s.size())
    if (s.size() < t.size()) {
        for (size_t i = 0; i < t.size() - s.size() + 1; ++i) {  // O(t.size())
            bool flag = true;
            for (size_t q = 0; q < s.size(); ++q) {
                if (t[i + q] != s[q]) {  // O(s.size())
                    flag = false;
                    break;
                }
            }
            if (flag) {
                ans->emplace_back(i + 1);
            }
        }
    }
}
 
const string FILE_NAME = "kek";
 
int main() {
    // freopen((FILE_NAME + ".in").c_str(), "down", stdin);
    // freopen((FILE_NAME + ".out").c_str(), "w", stdout);
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    string s, t;
    cin >> s >> t;
    vector<size_t> ans;
    findAllSubStr(&ans, s, t);
    cout << ans.size() << '\n';
    for (auto e : ans) {
        cout << e << ' ';
    }
    return 0;
}