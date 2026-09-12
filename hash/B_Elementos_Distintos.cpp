#include <iostream>
#include <unordered_map>
using namespace std;

using u64 = unsigned long long;
using i64 = long long;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    unordered_map<i64, bool> visto;
    visto.reserve(n * 2);   // evita rehashes, acelera la inserción

    for (int i = 0; i < n; i++) {
        i64 x;
        cin >> x;
        visto[x] = true;    // el valor no importa, lo único relevante es la key
    }

    cout << visto.size() << "\n";
    return 0;
}
