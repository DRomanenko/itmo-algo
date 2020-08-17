#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

vector<int> Stack;
int Size = 0;

void push (int a) {
    Stack[++Size] = a;
}

void pop () {
    Size--;
}

int get () {
    return Stack[Size];
}

int getSize () {
    return Size;
}

int main() {
    iostream::sync_with_stdio(false), cin.tie(nullptr);
    int n;
    cin >> n;
    vector<int> a(n), sortA(n);
    Stack.resize(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
        sortA[i] = a[i];
    }
    sort(sortA.begin(), sortA.end());
    int j = 0;
    string ans = "";
    for (int i = 0; i < n; ++i) {
        push(a[i]);
        ans += "push\n";
        while (get() == sortA[j] && j < n) {
            j++;
            ans += "pop\n";
            pop();
        }
    }
    if (getSize() == 0) {
        cout << ans;
    } else {
        cout << "impossible";
    }
    return 0;
}