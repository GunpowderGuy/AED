#include<bits/stdc++.h>
using namespace::std;

int main() {
    cin.tie(0) -> sync_with_stdio(false);
    int A, B, x, y;
    cin >> A >> B >> x >> y;
    int edges = 0;
    int target_in_degree = 0;
    for (int i = 0; i <= A; ++i) {
        for (int j = 0; j <= B; ++j) {
            int new_i = A;
            int new_j = j;
            if (new_i != i) {
                ++edges;
                if (new_i == x and new_j == y) {
                    ++target_in_degree;
                }
            }
            new_i = i;
            new_j = B;
            if (new_j != j) {
                ++edges;
                if (new_i == x and new_j == y) {
                    ++target_in_degree;
                }
            }

            new_i = 0;
            new_j = j;
            if (new_i != i) {
                ++edges;
                if (new_i == x and new_j == y) {
                    ++target_in_degree;
                }
            }
            new_i = i;
            new_j = 0;
            if (new_j != j) {
                ++edges;
                if (new_i == x and new_j == y) {
                    ++target_in_degree;
                }
            }
            int transfer = min(i, B - j);
            new_i = i - transfer;
            new_j = j + transfer;
            if (new_i != i or new_j != j) {
                ++edges;
                if (new_i == x and new_j == y) {
                    ++target_in_degree;
                }
            }
            transfer = min(A - i, j);
            new_i = i + transfer;
            new_j = j - transfer;
            if (new_i != i or new_j != j) {
                ++edges;
                if (new_i == x and new_j == y) {
                    ++target_in_degree;
                }
            }
        }
    }
    cout << edges << " " << target_in_degree << '\n';
    return 0;
}