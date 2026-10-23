/*

//#include <bits/stdc++.h>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Devuelve todos los códigos de longitud n sin unos consecutivos,
// en orden lexicográfico (0 antes que 1).
vector<string> codigos_validos(int n) {
    vector<string> res;
    int total = 1 << n;  // 2^n cadenas de n bits
    for (int mascara = 0; mascara < total; ++mascara) {
        string s(n, '0');
        for (int i = 0; i < n; ++i)
            if (mascara & (1 << i))
                s[n - 1 - i] = '1';   // bit más significativo queda a la izquierda
        // Descartar si hay dos '1' seguidos
        if (s.find("11") == string::npos)
            res.push_back(s);
    }
    return res;
}

int main() {
    int n;
    cin >> n;
    vector<string> codigos = codigos_validos(n);
    cout << codigos.size() << '\n';
    for (const string& s : codigos)
        cout << s << '\n';
    return 0;
}

*/

/*
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Devuelve todos los códigos válidos (longitud n, sin unos consecutivos)
// en orden lexicográfico, como vectores de bool.
vector<vector<bool>> codigos_validos(const std::size_t n) {
    vector<vector<bool>> res;
    int total = 1 << n;  // 2^n
    for (int mascara = 0; mascara < total; ++mascara) {
        vector<bool> bits(n, false);
        for (int i = 0; i < n; ++i)
            if (mascara & (1 << i))
                bits[n - 1 - i] = true;  // bit más significativo a la izquierda
        // Descartar si hay dos 'true' consecutivos
        bool valido = true;
        for (int i = 0; i + 1 < n; ++i)
            if (bits[i] && bits[i + 1]) {
                valido = false;
                break;
            }
        if (valido)
            res.push_back(bits);
    }
    return res;
}

// Conversión a string: false -> '0', true -> '1'
string a_string(const vector<bool>& bits) {
    string s(bits.size(), '0');
    for (size_t i = 0; i < bits.size(); ++i)
        if (bits[i])
            s[i] = '1';
    return s;
}

int main() {
    int n;
    cin >> n;
    vector<vector<bool>> codigos = codigos_validos(n);
    cout << codigos.size() << '\n';
    for (const auto& c : codigos)
        cout << a_string(c) << '\n';
    return 0;
}

*/


#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Genera recursivamente todos los códigos válidos de longitud n.
// Sufijo "valido hasta ahora": solo extendemos prefijos que ya cumplen.
void generar(vector<bool>& prefijo, int n, vector<vector<bool>>& res) {
    if ((int)prefijo.size() == n) {
        res.push_back(prefijo);
        return;
    }
    // Rama 0: siempre permitida
    prefijo.push_back(false);
    generar(prefijo, n, res);
    prefijo.pop_back();
    // Rama 1: solo si el último elemento no es 1
    if (prefijo.empty() || !prefijo.back()) {
        prefijo.push_back(true);
        generar(prefijo, n, res);
        prefijo.pop_back();
    }
}

// Punto de entrada: devuelve todos los códigos válidos en orden lexicográfico.
vector<vector<bool>> codigos_validos(int n) {
    vector<vector<bool>> res;
    vector<bool> prefijo;
    prefijo.reserve(n);
    generar(prefijo, n, res);
    return res;
}

// Conversión a string: false -> '0', true -> '1' (solo para el IO)
string a_string(const vector<bool>& bits) {
    string s(bits.size(), '0');
    for (size_t i = 0; i < bits.size(); ++i)
        if (bits[i])
            s[i] = '1';
    return s;
}

int main() {
    int n;
    cin >> n;
    vector<vector<bool>> codigos = codigos_validos(n);
    cout << codigos.size() << '\n';
    for (const auto& c : codigos)
        cout << a_string(c) << '\n';
    return 0;
}
