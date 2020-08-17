#include <bits/stdc++.h>

using namespace std;

int main() {
    iostream::sync_with_stdio(false), cin.tie(nullptr);
	int n;
	cin >> n;

	deque<int> DequeFirst, DequeSecond;
	for (int i = 0; i < n; ++i) {
	    char a;
        cin >> a;
        if (a == '+') {
            int id;
            cin >> id;
            DequeSecond.push_back(id);
        }
        if (a == '*') {
            int q;
            cin >> q;
            if (DequeFirst.size() <= DequeSecond.size()) {
                DequeFirst.push_back(q);
            } else {
                DequeSecond.push_front(q);
            }
        }
        if (a == '-') {
            cout << DequeFirst.front() << endl;
            DequeFirst.pop_front();
        }

        if (DequeFirst.size() < DequeSecond.size()) {
            DequeFirst.push_back(DequeSecond.front());
            DequeSecond.pop_front();
        }
	}
	return 0;
}