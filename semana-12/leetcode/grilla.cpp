// Shortest Path in Binary Matrix (LeetCode 1091)
// BFS con contagio de distancias desde el punto inicial.
//
// Convención de la grilla:
//   - nullopt  -> celda bloqueada (obstáculo)
//   - valor 0  -> celda libre, aún sin visitar
//   - valor d>0 -> distancia mínima desde el inicio (en celdas, incluyendo la inicial)
//
// Sin I/O: la grilla se construye/consuma desde fuera.

#include <array>
#include <cstddef>
#include <optional>
#include <queue>
#include <utility>
#include <vector>

namespace {

using Distance = size_t;  // 0 = sin visitar; >=1 = distancia asignada
using Cell = std::optional<Distance>;
using Grid = std::vector<std::vector<Cell>>;
using Position = std::pair<std::size_t, std::size_t>;

// Los 8 offsets adyacentes al centro (fila, columna).
constexpr std::array<Position, 8> kOffsets{{
    {0, 1}, {1, 1}, {1, 0}, {1, -1},
    {0, -1}, {-1, -1}, {-1, 0}, {-1, 1},
}};

bool existe(const Grid& grilla, const std::size_t fila, const std::size_t columna) {
    return fila < grilla.size() && columna < grilla[fila].size();
}

// Un vecino solo es transitable si existe, no está bloqueado y no fue visitado.
bool es_transitable(const Grid& grilla, const std::size_t fila, const std::size_t columna) {
    return existe(grilla, fila, columna) && grilla[fila][columna].has_value() &&
           *grilla[fila][columna] == 0;
}

// Expande una celda: devuelve sus vecinos transitables.
std::vector<Position> vecinos_transitables(const Grid& grilla, const Position celda) {
    const auto [fila, columna] = celda;
    std::vector<Position> resultado;
    resultado.reserve(kOffsets.size());
    for (const auto& [df, dc] : kOffsets) {
        const std::size_t nf = fila + df;
        const std::size_t nc = columna + dc;
        if (es_transitable(grilla, nf, nc)) {
            resultado.emplace_back(nf, nc);
        }
    }
    return resultado;
}

// Propaga la distancia de `actual` a sus vecinos transitables y los encola.
void contagiar_vecinos(Grid& grilla, std::queue<Position>& cola, const Position actual) {
    const Distance siguiente = *grilla[actual.first][actual.second] + 1;
    for (const auto& vecino : vecinos_transitables(grilla, actual)) {
        grilla[vecino.first][vecino.second] = siguiente;
        cola.push(vecino);
    }
}

}  // namespace

// Devuelve la longitud del camino más corto entre `inicio` y `destino`,
// o nullopt si no existe camino. Modifica la grilla con las distancias.
std::optional<std::size_t> camino_mas_corto(Grid& grilla, const Position inicio, const Position destino) {
    const bool extremos_bloqueados =
        !existe(grilla, inicio.first, inicio.second) ||
        !grilla[inicio.first][inicio.second].has_value() ||
        !existe(grilla, destino.first, destino.second) ||
        !grilla[destino.first][destino.second].has_value();
    if (extremos_bloqueados) {
        return std::nullopt;
    }

    grilla[inicio.first][inicio.second] = 1;  // la distancia cuenta celdas, incluida la inicial
    std::queue<Position> cola;
    cola.push(inicio);

    while (!cola.empty()) {
        const Position actual = cola.front();
        cola.pop();
        if (actual == destino) {
            return static_cast<std::size_t>(*grilla[actual.first][actual.second]);
        }
        contagiar_vecinos(grilla, cola, actual);
    }
    return std::nullopt;
}


#include <cstddef>
#include <iostream>

int main() {
    std::size_t n{};
    if (!(std::cin >> n)) {
        return 0;
    }

    Grid grilla;
    grilla.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        std::vector<Cell> fila;
        fila.reserve(n);
        for (std::size_t j = 0; j < n; ++j) {
            int valor{};
            std::cin >> valor;
            // 1 (obstáculo de LeetCode) -> nullopt; 0 (libre) -> 0 sin visitar
            fila.push_back(valor == 1 ? std::nullopt : std::optional<Distance>{0});
        }
        grilla.push_back(std::move(fila));
    }

    const auto resultado =
        camino_mas_corto(grilla, {0, 0}, {n - 1, n - 1});

    if (resultado.has_value()) {
        std::cout << *resultado << '\n';
    } else {
        std::cout << -1 << '\n';
    }
    return 0;
}
