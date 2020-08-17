/*Для решения этой задачи применим метод динамического программирования. Пусть b[d][k] – количество номеров, набираемых ходом коня, которые начинаются с цифры d и состоят из k цифр. Тогда b[d][1]=1 для всех d, а b[d][k] для любого d вычисляется через сумму b[i][k-1] для k>1. Так, например, b[4][k] = b[0][k-1]+b[3][k-1]+b[9][k-1]. Увеличивая k от 2 до n мы получим значения b[d][n], сумма которых (за вычетом b[0][n] и b[8][n]) и даст ответ на поставленную задачу.

Заметим также, что при вычислении могут получаться достаточно большие значения, поэтому следует применить длинную арифметику.*/
#include <bits/stdc++.h>

using namespace std;

#define ll long long
const ll MOD = static_cast<const long long int>(1E9);

int main () {
    iostream::sync_with_stdio(false), cin.tie(nullptr);

    unsigned int n;
    cin >> n;

    vector<vector<unsigned ll>> d(10, vector<unsigned ll>(n + 1));
    // 1 - нач. цифра; 2 - количество цифр

    // База
    for (int i = 0; i < 10; ++i) {
        d[i][1] = 1;
    }

    //Переход
    for (int i = 2; i <= n; ++i) {
        for (int q = 0; q < 10; ++q) {
            if (q == 0) {
                d[q][i] = (d[4][i - 1] + d[6][i - 1]) % MOD;
            }
            if (q == 1) {
                d[q][i] = (d[6][i - 1] + d[8][i - 1]) % MOD;
            }
            if (q == 2) {
                d[q][i] = (d[7][i - 1] + d[9][i - 1]) % MOD;
            }
            if (q == 3) {
                d[q][i] = (d[4][i - 1] + d[8][i - 1]) % MOD;
            }
            if (q == 4) {
                d[q][i] = ((d[0][i - 1] + d[3][i - 1]) % MOD + d[9][i - 1]) % MOD;
            }
            if (q == 5) {
                d[q][i] = 0;
            }
            if (q == 6) {
                d[q][i] = ((d[0][i - 1] + d[1][i - 1]) % MOD + d[7][i - 1]) % MOD;
            }
            if (q == 7) {
                d[q][i] = (d[2][i - 1] + d[6][i - 1]) % MOD;
            }
            if (q == 8) {
                d[q][i] = (d[1][i - 1] + d[3][i - 1]) % MOD;
            }
            if (q == 9) {
                d[q][i] = (d[2][i - 1] + d[4][i - 1]) % MOD;
            }
        }
    }
    unsigned ll sum = 0;
    for (int i = 0; i < 10; ++i) {
        if (i != 0 && i != 8) {
            sum = (sum + d[i][n]) % MOD;
        }
    }
    cout << sum << endl;
    return 0;
}	