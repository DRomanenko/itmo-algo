#include <bits/stdc++.h>

using namespace std;

int main() {
    iostream::sync_with_stdio(false), cin.tie(nullptr);
	int n;
	cin >> n;

	list<int> Deque;
	for (int i = 0; i < n; ++i) {
        int a;
        cin >> a;
        if (a == 1) {
            int id;
            cin >> id;
            Deque.push_back(id);
        }
        if (a == 2) {
            Deque.pop_front();
        }
        if (a == 3) {
            Deque.pop_back();
        }
        if (a == 4) {
            int q;
            cin >> q;
            int count = 0;
            for (list<int>::iterator i = Deque.begin(); i != Deque.end(); ++i) {
                if (*i == q) {
                    break;
                }
                count++;
            }
            cout << count << endl;
        }
        if (a == 5) {
            cout << Deque.front() << endl;
        }
	}
	return 0;
}