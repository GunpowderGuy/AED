#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum class EstadoCasilla {
    Bloqueada,
    Libre,
    Visitada
};

const int DF[4] = {-1, 1, 0, 0};
const int DC[4] = {0, 0, -1, 1};

EstadoCasilla de_char(char c) {
    return c == '#' ? EstadoCasilla::Bloqueada : EstadoCasilla::Libre;
}

int64_t contar(int f, int c, int n, int m,
               vector<vector<EstadoCasilla>>& grilla) {
    if (f == n - 1 && c == m - 1)
        return 1;
    grilla[f][c] = EstadoCasilla::Visitada;
    int64_t total = 0;
    for (int k = 0; k < 4; ++k) {
        int nf = f + DF[k];
        int nc = c + DC[k];
        if (nf >= 0 && nf < n && nc >= 0 && nc < m &&
            grilla[nf][nc] == EstadoCasilla::Libre)
            total += contar(nf, nc, n, m, grilla);
    }
    grilla[f][c] = EstadoCasilla::Libre;
    return total;
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<EstadoCasilla>> grilla(n, vector<EstadoCasilla>(m));
    string fila;
    for (int i = 0; i < n; ++i) {
        cin >> fila;
        for (int j = 0; j < m; ++j)
            grilla[i][j] = de_char(fila[j]);
    }
    cout << contar(0, 0, n, m, grilla) << '\n';
    return 0;
}
