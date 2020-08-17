#include <vector>
#include <iostream>

using namespace std;

vector<pair<int, int>> Stack;
int Size = 0;

void push (int a) {
    Stack[++Size] = {a, Size == 0 ? a : min(a, Stack[Size].second)};
}

void pop () {
    Size--;
}

int getMin () {
    return Stack[Size].second;
}

int main() {
    iostream::sync_with_stdio(false), cin.tie(nullptr);
    int n;
    cin >> n;
    Stack.resize(n);
    for (int i = 0; i < n; ++i) {
        int a;
        cin >> a;
        if (a == 1) {
            int b;
            cin >> b;
            push(b);
        }
        if (a == 2) {
            pop();
        }
        if (a == 3) {
            cout << getMin() << endl;
        }
    }
    return 0;
}