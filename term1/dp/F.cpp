#include <bits/stdc++.h>

using namespace std;

#define unshort unsigned short
const unshort MAX = static_cast<const unsigned short>(6E5);

int main () {
    iostream::sync_with_stdio(false), cin.tie(nullptr);

    unshort n;
    cin >> n;

    vector<unshort> cost(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> cost[i];
    }

    vector<vector<unsigned int>> d(n + 1, vector<unsigned int>(n + 1, MAX));
    // d[i][q] - min сумма, которую мог потратить Петя за первые i дней,
    // при условии, что после этого у него в запасе осталось q талонов
    // на бесплатную еду.

    // База
    d[0][0] = 0;

    //Переход
    for (int i = 1; i <= n; ++i) {
        for (int q = 0; q <= n; ++q) {
            if (cost[i] <= 100) {
                d[i][q] = min((i > 0) ? (d[i - 1][q] + cost[i]) : MAX,
                              (i > 0 && q < n) ? d[i - 1][q + 1] : MAX);
                // d[i - 1][q + 1] - Петя использует талон на бесплатную еду
                // d[i - 1][q] + cost[i] - Петя покупает i-й обед
            }
            if (cost[i] > 100) {
                d[i][q] = min((i > 0 && q > 0) ? (d[i - 1][q - 1] + cost[i]) : MAX,
                              ((i > 0 && q < n) ? d[i - 1][q + 1] : MAX));
                // d[i - 1][q + 1] - Петя использует талон на бесплатную еду
                // d[i - 1][q - 1] + cost[i] - Петя покупает i-й обед и при этом получает
                // талон на бесплатную еду
            }
        }
    }

    // Восстановление ответа
    unshort ans = MAX, saveIndex = 0;
    for (unshort i = 0; i <= n; ++i) {
        if (d[n][i] < ans) {
            ans = d[n][i];
            saveIndex = i;
        }
        if (d[n][i] == ans) {
            saveIndex = i;
        }
    }
    vector<unshort> save;
    int i = n, q = saveIndex;
    do {
        if ((i > 0) && d[i][q] == d[i - 1][q] + cost[i]) {
            i--;
            continue;
        }
        if ((i > 0 && q > 0) && d[i][q] == d[i - 1][q - 1] + cost[i]) {
            i--;
            q--;
            continue;
        }
        if (d[i][q] == d[i - 1][q + 1]) {
            i--;
            q++;
            save.push_back(i);
            continue;
        }
    } while (i > 0);
    cout << ans << endl << saveIndex << " " << save.size() << endl;
    for (i = save.size() - 1; i >= 0; --i) {
        cout << save[i] + 1 << endl;
    }

    return 0;
}