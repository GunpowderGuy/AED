#include <bits/stdc++.h>
using namespace std;

vector<size_t> calcular_grados(
    size_t n,
    size_t m,
    const vector<pair<size_t, size_t>>& aristas
) {
    vector<size_t> grados(n + 1, 0);

    for (size_t i = 0; i < m; ++i) {
        auto [u, v] = aristas[i];
        ++grados[u];
        ++grados[v];
    }

    return grados;
}

enum class Resultado {
    CIRCUITO,
    CAMINO,
    IMPOSIBLE
};

Resultado clasificar_grafo(const vector<size_t>& grados) {
    size_t impares = 0;

    for (size_t i = 1; i < grados.size(); ++i) {
        if (grados[i] % 2 != 0) {
            ++impares;
        }
    }

    if (impares == 0) return Resultado::CIRCUITO;
    if (impares == 2) return Resultado::CAMINO;
    return Resultado::IMPOSIBLE;
}

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);

    size_t n, m;
    cin >> n >> m;

    vector<pair<size_t, size_t>> aristas(m);

    for (auto& [u, v] : aristas) {
        cin >> u >> v;
    }

    vector<size_t> grados = calcular_grados(n, m, aristas);
    Resultado resultado = clasificar_grafo(grados);

    switch (resultado) {
        case Resultado::CIRCUITO:
            cout << "CIRCUITO\n";
            break;
        case Resultado::CAMINO:
            cout << "CAMINO\n";
            break;
        case Resultado::IMPOSIBLE:
            cout << "IMPOSIBLE\n";
            break;
    }

    return 0;
}
