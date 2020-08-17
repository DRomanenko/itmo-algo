#include <bits/stdc++.h>

using namespace std;

const short MAX = static_cast<const short>(3E5);

void f (vector<vector<short>> &d, vector<vector<short>> &p, string s, short l, short r) {
    if (!d[l][r]) {
        for (int i = l; i < r + 1; ++i) {
            cout << s[i];
        }
        return;
    }
    if (d[l][r] == r - l + 1) {
        return;
    }
    if (p[l][r] == -1) {
        // подстрока имеет вложенную ПСП
        cout << s[l];
        f(d, p, s, l + 1, r - 1);
        cout << s[r];
        return;
    }
    f(d, p, s, l, p[l][r]); // левая подстрока
    f(d, p, s, p[l][r] + 1, r); // правая подстрока
}

int main () {
    iostream::sync_with_stdio(false), cin.tie(nullptr);

    string s;
    cin >> s;

    vector<vector<short>> d(s.size(), vector<short>(s.size(), 0)),
            p(s.size(), vector<short>(s.size()));
    /*  d[i][q] - наимаеньшее количество символов , которые надо удалить из S,
        чтобы получилась ПСП, левый сивол которой имеет позицию i, а правый j.
    */

    // База
    for (short i = 0; i < s.size(); ++i) {
        d[i][i] = 1;
    }

    //Переход
    for (short r = 0; r < s.size(); ++r) {
        for (short l = r; l >= 0; --l) {
            if (l != r) {
                short save = MAX, saveIndex = -1;
                if ((s[l] == '(' && s[r] == ')') ||
                    (s[l] == '{' && s[r] == '}') ||
                    (s[l] == '[' && s[r] == ']')) {
                    save = d[l + 1][r - 1];
                }
                for (short i = l; i < r; ++i) {
                    if (d[l][i] + d[i + 1][r] < save) {
                        save = d[l][i] + d[i + 1][r];
                        saveIndex = i;
                    }
                }
                p[l][r] = saveIndex;
                d[l][r] = save;
            }
        }
    }

    // Восстановление ответа
    f(d, p, s, 0, s.size() - 1);
    return 0;
}