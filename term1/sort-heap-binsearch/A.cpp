#include <bits/stdc++.h>

typedef long long ll;
typedef long double ld;

using namespace std;

void QuickSort (vector<int> &a, int b, int e) {
    int l = b, r = e;
    int pivot = a[l + rand() % (r - l)];
    while (l <= r) {
        while (a[l] < pivot)
            l++;
        while (a[r] > pivot)
            r--;
        if (l <= r)
            swap (a[l++], a[r--]);
    }
    if (b < r)
        QuickSort (a, b, r);
    if (e > l)
        QuickSort (a, l, e);
}

int main(void){
    iostream::sync_with_stdio(0), cin.tie(0);
    int n;

    cin >> n;

    vector<int> array(n);

    for (int i = 0; i < n; ++i) {
        cin >> array[i];
    }

    QuickSort (array, 0, n - 1);

    for (int i = 0; i < n; ++i) {
        cout << array[i] << ' ';
    }
    cout << "\n";

    return 0;
}