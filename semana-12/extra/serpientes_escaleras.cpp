/*

  Problema: serpientes y escaleras (Codeforces, grupo nylekuuZuw, concurso 728509)
 
  Tablero de N casillas (1..N). Ian empieza en la casilla 1 y quiere llegar
  exactamente a la N. En cada turno elige un d entre 1 y D y avanza de x a
  x + d (si no se pasa de N). Hay k casillas especiales: caer exactamente
  en a_i lo teleporta a b_i en el mismo turno (escalera si b_i > a_i,
  serpiente si b_i < a_i). No hay cadenas: ningun b_i es a su vez un a_j.
  Hallar la minima cantidad de turnos, o -1 si es imposible.
 
  Idea: BFS por contagio sobre las casillas. Estado = casilla actual; desde x
  los vecinos son x+d (1 <= d <= D) mapeados por el teleport: si x+d es un
  a_i, el vecino real es b_i. Como todas las jugadas cuestan 1 turno, BFS
  clasico da el minimo. Las casillas a_i nunca quedan como estado (siempre
  se resuelven al aterrizar), asi que el mapeo salto[y] se aplica al encolar.
 */

#include <iostream>
#include <optional>
#include <queue>
#include <utility>
#include <vector>

namespace {

// salto[y] = casilla donde terminas si aterrizas en y (y si no es especial).
std::vector<int> construir_salto(const int n,
                                 const std::vector<std::pair<int, int>>& especiales) {
    std::vector<int> salto(n + 1);
    for (int i = 1; i <= n; ++i) {
        salto[i] = i;
    }
    for (const auto& [a, b] : especiales) {
        salto[a] = b;
    }
    return salto;
}

// Casillas donde puedes aterrizar tirando el dado desde x (sin aplicar teleport).
std::vector<int> aterrizajes(const int x, const int dado, const int n) {
    std::vector<int> resultado;
    resultado.reserve(dado);
    for (int d = 1; d <= dado; ++d) {
        if (x + d <= n) {
            resultado.push_back(x + d);
        }
    }
    return resultado;
}

// BFS por contagio: devuelve los turnos minimos para llegar a n, o nullopt.
std::optional<int> turnos_minimos(const int n, const int dado,
                                  const std::vector<int>& salto) {
    std::vector<int> distancia(n + 1, -1);  // -1 = sin visitar
    std::queue<int> cola;
    distancia[1] = 0;
    cola.push(1);

    while (!cola.empty()) {
        const int actual = cola.front();
        cola.pop();
        for (const int y : aterrizajes(actual, dado, n)) {
            const int vecino = salto[y];  // teleport si y es especial
            if (distancia[vecino] != -1) {
                continue;
            }
            const int siguiente = distancia[actual] + 1;
            if (vecino == n) {
                return siguiente;
            }
            distancia[vecino] = siguiente;
            cola.push(vecino);
        }
    }
    return std::nullopt;
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);

    int n, dado, k;
    std::cin >> n >> dado >> k;

    std::vector<std::pair<int, int>> especiales;
    especiales.reserve(k);
    for (int i = 0; i < k; ++i) {
        int a, b;
        std::cin >> a >> b;
        especiales.emplace_back(a, b);
    }

    const std::vector<int> salto = construir_salto(n, especiales);
    const auto resultado = turnos_minimos(n, dado, salto);
    std::cout << (resultado.has_value() ? *resultado : -1) << '\n';
    return 0;
}
