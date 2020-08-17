#include <bits/stdc++.h>

using namespace std;

vector<int> Stack (200);
int Size = 0;

void push (int a) {
    Stack[Size++] = a;
}

int pop () {
    return Stack[--Size];
}

int main() {
    iostream::sync_with_stdio(false), cin.tie(nullptr);
    char a;
    while (cin >> a) {
        if (a >= '0' && a <= '9') {
            push(a - '0');
        }
        if (a == '+') {
            push(pop() + pop());
        }
        if (a == '-') {
            int a = pop(), b = pop();
            push(b - a);
        }
        if (a == '*') {
            push(pop() * pop());
        }
    }
    cout << pop();
    return 0;
}