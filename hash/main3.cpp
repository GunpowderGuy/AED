#include <bits/stdc++.h>
using namespace std;

template<typename key_type, typename value_type>
struct my_map {
    int m;
    vector<vector<pair<key_type, value_type>>> chains;

    my_map(int m) : m(m) {
        chains.resize(m);
    }

    int _hash(key_type key) const {
        long long k = key;
        if (k < 0) k = -k;              // aceptar negativos (por si acaso)
        const long long B = 311;
        const long long MOD = 1000000007LL;
        long long hash_value = 0;
        while (k > 0) {
            int d = k % 10;
            hash_value = (hash_value * B + (d + 1)) % MOD;
            k /= 10;
        }
        return (int)(hash_value % m);
    }

    value_type& operator[](const key_type &key) {
        int chain_position = _hash(key);
        int at = 0;
        int sz = (int)chains[chain_position].size();
        while (at < sz && chains[chain_position][at].first != key) {
            ++at;
        }
        if (at == sz) {
            chains[chain_position].emplace_back(key, value_type());
            return chains[chain_position].back().second;
        }
        return chains[chain_position][at].second;
    }

    bool has_key(const key_type &key) const {
        int chain_position = _hash(key);
        for (const auto &e : chains[chain_position]) {
            if (e.first == key) return true;
        }
        return false;
    }
};

using i64 = long long;

bool complementoXOR(const vector<i64>& a, const i64 T) {
    my_map<i64, bool> mapa(999983);

    for (i64 x : a) {
        i64 complemento = x ^ T;
        if (mapa.has_key(complemento)) {
            return true;
        }
        mapa[x] = true;
    }
    return false;
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


    cout << (complementoXOR(a, T) ? "SI" : "NO") << endl;

    return 0;
}
