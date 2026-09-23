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
     / / Para entero*s
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



//  using i64 = long long;
using i64 = int;

bool complementoXOR(const vector<i64>& a, const i64 T) {
    my_map<i64, bool> mapa(999983);   // value bool: "ya lo vi" (aqui el value no importa)
//~ negar bit por bit
//^ XOR bit por bit
   for (i64 x : a) {
       i64 complemento = ~x;

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

    cout <<T << endl;
    for (auto elem : a) {
        cout << elem << " ";
    }
    cout << (complementoXOR(a, T) ? "SI" : "NO") << endl;

    return 0;
}


/*
 B . Suma xor                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        *
 time limit per test
 3 seconds
 memory limit per test
 256 megabytes

 Se te da un arreglo de n enteros a1,…,an y un entero X (el valor objetivo). Determina si existen dos índices distintos i≠j tales que ai⊕aj=X, donde ⊕ denota la operación XOR bit a bit.
 Input

 La primera línea contiene dos enteros n y X (1≤n≤106, 0≤X≤109). La segunda línea contiene n enteros a1,…,an (0≤ai≤109): el arreglo.
 Output

 Imprime SI si existen dos índices distintos i≠j con ai⊕aj=X, o NO en caso contrario.
 Scoring

 Límites

 Este problema tiene subtareas, por lo cual es posible obtener un puntaje en base a las subtareas que logren ser resueltas.

 1≤n≤106
 0≤ai≤109 para todo i=1,…,n.
 0≤X≤109

 Grupo 1 (1 punto)

 n≤2

 Grupo 2 (1 punto)

 n≤2000

 Grupo 3 (2 puntos)

 Sin restricciones adicionales.

 Example
 Input
 Copy

 6 6
 3 7 10 1 9 12

 Output
 Copy

 SI

 Note

 Con n=6, X=6 y a=[3,7,10,1,9,12]: recorriendo el arreglo de izquierda a derecha y guardando los valores ya vistos, al llegar a a4=1 se calcula a4⊕X=00012⊕01102=01112=7, y 7 ya había aparecido antes (es a2), así que los índices i=2, j=4 cumplen a2⊕a4=01112⊕00012=01102=6=X y la respuesta es SI.

 Esto funciona porque a⊕b=X⟺a⊕X=b (el XOR es su propio inverso), lo que da la misma forma de "buscar el complemento en un conjunto de vistos" que un problema de suma de dos términos, pero con XOR en vez de resta. Un detalle importante: si X=0, entonces ai⊕X=ai, así que revisar si ai ya fue visto antes de insertarlo es exactamente lo que se necesita para exigir una aparición distinta de ese mismo valor (dos posiciones con el mismo número, no una posición emparejada consigo misma).
*/
