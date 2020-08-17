#include <bits/stdc++.h>

using namespace std;

int main() {
    iostream::sync_with_stdio(false), cin.tie(nullptr);
	int n;
	cin >> n;

	list<pair<int, int>> ball;
	ball.push_back({-1, 1});

	for (int i = 0; i < n; ++i) {
        int a;
        cin >> a;
        if (ball.back().first == a) {
            ball.back().second++;
        } else {
            ball.push_back({a, 1});
        }
	}

	int count = 0;
	while (true) {
	    list<pair<int, int>>::iterator b = ball.end();
        for (list<pair<int, int>>::iterator i = ball.begin(); i != ball.end(); ++i) {
            if (i -> second >= 3) {
                b = i;
                break;
            }
        }
        if (b == ball.end()) {
            break;
        }
        count += b -> second;
        list<pair<int, int>>::iterator a = --b;
        b++;
        list<pair<int, int>>::iterator c = ++b;
        --b;
        if (a -> first == c -> first) {
            a -> second += c -> second;
            ball.erase(c);
        }
        ball.erase(b);
	}

	cout << count;
	return 0;
}