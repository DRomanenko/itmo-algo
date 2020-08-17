#include <bits/stdc++.h>

typedef long long ll;
typedef long double ld;

using namespace std;

vector<int> heap;
int n = 0;

void Insert (int value) {
    if (heap.size() <= n) {
        heap.push_back(value);
        n++;
    } else {
        heap[n++] = value;
    }
    int i = n - 1;
    while (i > 0 && heap[(i - 1) / 2] < heap[i]) {
        swap (heap[(i - 1) / 2], heap[i]);
        i = (i - 1) / 2;
    }
}

int Extract () {
    int res = heap[0];
    swap (heap[0], heap[n - 1]);
    n--;
    int i = 0;
    while (2 * i + 1 < n) {
        int j = 2 * i + 1;
        if (2 * i + 2 < n && heap[2 * i + 2] > heap[j]) {
            j = 2 * i + 2;
        }
        if (heap[j] > heap[i]) {
            swap (heap[i], heap[j]);
            i = j;
        } else {
            break;
        }
    }
    return res;
}

int main(void){
    iostream::sync_with_stdio(0), cin.tie(0);
    int n;
    cin >> n;

    for (int i = 0; i < n; ++i) {
        int operation;
        cin >> operation;

        if (operation) {
            cout << Extract() << "\n";
        } else {
            int value;
            cin >> value;
            Insert(value);
        }
    }
    return 0;
}