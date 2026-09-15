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
    /* int _hash(key_type key) const {
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
    } */

    int _hash(key_type key) const {
        if (key < 0) key = -key;    // aceptar negativos
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


bool suma_de_dos(const vector<i64>& a, const i64 T) {
    my_map<i64, bool> mapa(999983);   // value bool: "ya lo vi" (aqui el value no importa)

    for (i64 x : a) {
        i64 complemento = T - x;      // el valor que necesito para sumar T

        if (mapa.has_key(complemento)) {
            return true;              // aparecio antes en una posicion i < j
        }

        mapa[x] = true;               // ahora SI inserto el actual, para futuras iteraciones
    }

    return false;                     // ningun par cumplio
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    i64 T;
    cin >> n >> T;

    vector<i64> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    cout << (suma_de_dos(a, T) ? "SI" : "NO") << endl;

    return 0;
}
