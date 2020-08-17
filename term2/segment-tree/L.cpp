#include <iostream>
#include <vector>

using namespace std;

unsigned int n;

vector<vector<vector<long long>>> fenwick_tree;

void add (unsigned long long i, unsigned long long j, unsigned long long q, long long k) {
    long long x = i;
    while (x < fenwick_tree.size()) {
        long long y = j;
        while (y < fenwick_tree[x].size()) {
            long long z = q;
            while (z < fenwick_tree[x][y].size()) {
                fenwick_tree[x][y][z] += k;

                z = z | (z + 1);
            }
            y = y | (y + 1);
        }
        x = x | (x + 1);
    }
}

long long sum (unsigned long long i, unsigned long long j, unsigned long long q) {
    unsigned long long sum = 0;
    long long x = i;
    while (x >= 0) {
        long long y = j;
        while (y >= 0) {
            long long z = q;
            while (z >= 0) {
                sum += fenwick_tree[x][y][z];
                z = (z & (z + 1)) - 1;
            }
            y = (y & (y + 1)) - 1;
        }
        x = (x & (x + 1)) - 1;
    }
    return sum;
}

long long find_sum (unsigned long long x_1, unsigned long long y_1, unsigned long long z_1, unsigned long long x_2,
                    unsigned long long y_2, unsigned long long z_2) {
    
    long long a = sum (x_2, y_2, z_2);
    long long b = sum (x_2, y_2, z_1 - 1);
    long long up = a - b;

    long long centre = sum (x_1 - 1, y_1 - 1, z_1 - 1);

    long long centre_slice = sum (x_1 - 1, y_1 - 1, z_2) - centre;
    long long right_slice = sum (x_2, y_1 - 1, z_2) - sum (x_2, y_1 - 1, z_1 - 1) - centre_slice;
    long long left_slice = sum (x_1 - 1, y_2, z_2) - sum (x_1 - 1, y_2, z_1 - 1) - centre_slice;

    long long ans = up - centre_slice - right_slice - left_slice;

    return ans;

}

int main () {
    iostream::sync_with_stdio(false), cin.tie(nullptr);
    cin >> n;

    fenwick_tree.resize(n, vector<vector<long long>> (n, vector<long long> (n, 0)));
    unsigned int m;
    while (cin >> m) {
        if (m == 1) {
            unsigned int x, y, z;
            int k;
            cin >> x >> y >> z >> k;
            add(x, y, z, k);
        }
        if (m == 2) {
            unsigned int x_1, y_1, z_1, x_2, y_2, z_2;
            cin >> x_1 >> y_1 >> z_1 >> x_2 >> y_2 >> z_2;
            cout << find_sum(x_1, y_1, z_1, x_2, y_2, z_2) << "\n";
        }
        if (m == 3) {
            break;
        }
    }

    return 0;
}