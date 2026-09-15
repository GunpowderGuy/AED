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



using i64 = long long;

i64 primer_no_repetido(const vector<i64>& a) {
    // ahora key es i64 y value tambien es i64
    my_map<i64, i64> mapa(999983);

    // Paso 1: contar frecuencias
    for (i64 x : a) {
        mapa[x]++;   // si no existe, operator[] lo crea con value_type() = 0 y luego ++
    }

    // Paso 2: recorrer el arreglo EN ORDEN
    for (i64 x : a) {
        if (mapa[x] == 1) return x;
    }

    return -1;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    vector<i64> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    cout << primer_no_repetido(a) << endl;

    return 0;
}
