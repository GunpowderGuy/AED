#include <bits/stdc++.h>
using namespace std;
template<typename key_type, typename value_type>
struct my_map {
    int m;
    vector<vector<pair<key_type, value_type>>> chains;
    my_map(int m) : m(m) {
        chains.resize(m);
    }
    value_type& operator [] (const key_type &key) {
        int chain_position = _hash(key);
        int at = 0;
        while (at < chains[chain_position].size() and chains[chain_position]
            [at].first != key) {
            ++at;
            }
            if (at == chains[chain_position].size()) {
                chains[chain_position].emplace_back(key, value_type());
            }
            return chains[chain_position][at].second;
    }
    bool has_key (const key_type &key) const {
        int chain_position = _hash(key);
        int at = 0;
        while (at < chains[chain_position].size() and chains[chain_position]
            [at].first != key) {
            ++at;
            }
            if (at == chains[chain_position].size()) {
                return false;
            }
            return true;
    }
    //int _hash(key_type key) {
     int _hash(key_type key) const {
        // Para enteros
        const int B = 311;
        const int MOD = 1e9 + 7;
        int hash_value = 0;
        while (key > 0) {
            int d = key % 10;
            hash_value = (1ll * hash_value * B + (d + 1)) % MOD;
            key /= 10;
        }
        return hash_value % m;
    }
    void print() {
        for (int i = 0; i < m; ++i) {
            cout << "Bucket " << i << ": " << endl;
            for (auto &e : chains[i]) {
                cout << e.first << " --> " << e.second << endl;
            }
            cout << "End bucket" << endl;
        }
    }
};



vector<int> interseccion(const vector<int>& a, const vector<int>& b) {
    my_map<int, bool> enA(1000003), yaPuesto(1000003);   // m primo y > n

    for (int x : a) enA[x] = true;

    vector<int> res;
    for (int x : b) {
        if (enA.has_key(x) and !yaPuesto.has_key(x)) {
            yaPuesto[x] = true;
            res.push_back(x);
        }
    }
    return res;
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    int m;
    cin >> m;
    vector<int> b(m);
    for (int i = 0; i < m; i++) cin >> b[i];

    vector<int> r = interseccion(a, b);   // sin repetidos
    sort(r.begin(), r.end());             // el problema pide orden creciente

    cout << r.size() << "\n";
    for (int i = 0; i < (int)r.size(); i++) {
        if (i > 0) cout << " ";
        cout << r[i];
    }
    cout << "\n";   // si k=0, esta línea queda vacía, como pide el enunciado

    return 0;
}
