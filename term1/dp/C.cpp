#include <bits/stdc++.h>

using namespace std;

#define ll long long

int main () {
    iostream::sync_with_stdio(false), cin.tie(nullptr);

    unsigned int n;
    cin >> n;

    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    // База
    vector<int> d(n, 1);
    vector<int> p(n, -1);

    //Переход
    for (int i = 0; i < n; ++i) {
        for (int q = 0; q < i; ++q) {
            if (a[q] < a[i]) { // слева элемент меньше текущего
                if (d[q] + 1 > d[i]) { // длина старой подпоследовательности больше текущей
                    d[i] = d[q] + 1;
                    p[i] = q;
                }
            }
        }
    }
    // Находим максимальную длинну подпоследовательности
    int currPos = 0, maxLength = 0;
    for (int i = 0; i < n; ++i) {
        if (d[i] > maxLength) {
            maxLength = d[i];
            currPos = i;
        }
    }
    cout << maxLength << endl; // длина НВП

    // Восстановление ответа
    vector<int> ans;
    while (currPos != -1) {
        ans.push_back(a[currPos]);
        currPos = p[currPos];
    }
    for (int i = ans.size() - 1; i >= 0; --i) {
        cout << ans[i] << " ";
    }
    return 0;
}