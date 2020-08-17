#include <bits/stdc++.h>

typedef long long ll;
typedef long double ld;

using namespace std;

void QuickSort (vector<int> &a, int b, int e) {
    int l = b, r = e;
    int pivot = a[l + rand() % (r - l + 1)];
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

int BinarySearch1 (vector<int> &a, int key) {
    int left = -1;
    int right = a.size();
    while (right > left + 1) {
        int middle = left + ((right - left) >> 1);
        if (a[middle] < key) {
            left = middle;
        }
        else {
            right = middle;
        }
    }
    return right;
}

int BinarySearch2 (vector<int> &a, int key) {
    int left = -1;
    int right = a.size();
    while (right > left + 1) {
        int middle = left + ((right - left) >> 1);
        if (a[middle] <= key) {
            left = middle;
        }
        else {
            right = middle;
        }
    }
    return left;
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

    int requests;

    cin >> requests;

    for (int i = 0; i < requests; ++i) {
        int l, r;
        cin >> l >> r;
        cout << BinarySearch2(array, r) - BinarySearch1(array, l) + 1 << ' ';
    }
    cout << "\n";

    return 0;
}