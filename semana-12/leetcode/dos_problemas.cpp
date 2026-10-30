// BFS en grillas: LeetCode 1091 y 1926 en un solo archivo.
//
// Convención interna de la grilla:
//   - nullopt   -> celda bloqueada (obstáculo)
//   - valor 0   -> celda libre, aún sin visitar
//   - valor d>0 -> distancia en pasos desde el inicio
//
// Uso: deja activa UNA sola clase Solution (la del problema que vas a probar
// en LeetCode) y comenta la otra.

//https://leetcode.com/problems/nearest-exit-from-entrance-in-maze/
//https://leetcode.com/problems/shortest-path-in-binary-matrix/description/

#include <array>
#include <cstddef>
#include <functional>
#include <optional>
#include <queue>
#include <utility>
#include <vector>

namespace {

using Distance = size_t;  // 0 = inicio (distancia en pasos)
using Cell = std::optional<Distance>;
using Grid = std::vector<std::vector<Cell>>;
using Position = std::pair<std::size_t, std::size_t>;

// Offsets adyacentes: 8 direcciones (1091) y 4 direcciones (1926).
constexpr std::array<Position, 8> kOffsets8{{
    {0, 1}, {1, 1}, {1, 0}, {1, -1},
    {0, -1}, {-1, -1}, {-1, 0}, {-1, 1},
}};
constexpr std::array<Position, 4> kOffsets4{{
    {0, 1}, {1, 0}, {0, -1}, {-1, 0},
}};

bool existe(const Grid& grilla, const std::size_t fila, const std::size_t columna) {
    return fila < grilla.size() && columna < grilla[fila].size();
}

// Un vecino solo es transitable si existe, no está bloqueado y no fue visitado.
bool es_transitable(const Grid& grilla, const std::size_t fila, const std::size_t columna) {
    return existe(grilla, fila, columna) && grilla[fila][columna].has_value() &&
           *grilla[fila][columna] == 0;
}

// Expande una celda con los offsets indicados: devuelve sus vecinos transitables.
template <std::size_t N>
std::vector<Position> vecinos_transitables(const Grid& grilla, const Position celda,
                                           const std::array<Position, N>& offsets) {
    const auto [fila, columna] = celda;
    std::vector<Position> resultado;
    resultado.reserve(offsets.size());
    for (const auto& [df, dc] : offsets) {
        const std::size_t nf = fila + df;
        const std::size_t nc = columna + dc;
        if (es_transitable(grilla, nf, nc)) {
            resultado.emplace_back(nf, nc);
        }
    }
    return resultado;
}

// Propaga la distancia de `actual` a sus vecinos transitables y los encola.
template <std::size_t N>
void contagiar_vecinos(Grid& grilla, std::queue<Position>& cola, const Position actual,
                       const std::array<Position, N>& offsets) {
    const Distance siguiente = *grilla[actual.first][actual.second] + 1;
    for (const auto& vecino : vecinos_transitables(grilla, actual, offsets)) {
        grilla[vecino.first][vecino.second] = siguiente;
        cola.push(vecino);
    }
}

bool es_borde(const Grid& grilla, const Position celda) {
    const auto [fila, columna] = celda;
    return fila == 0 || columna == 0 || fila == grilla.size() - 1 ||
           columna == grilla[fila].size() - 1;
}

// Traduce la grilla de 1091 (int: 0=libre, 1=bloqueado) a la convención interna.
// [[maybe_unused]]: solo se usa cuando la clase de 1091 está activa.
[[maybe_unused]] Grid traducir(const std::vector<std::vector<int>>& entrada) {
    Grid grilla;
    grilla.reserve(entrada.size());
    for (const auto& fila : entrada) {
        std::vector<Cell> nueva_fila;
        nueva_fila.reserve(fila.size());
        for (const int valor : fila) {
            nueva_fila.push_back(valor == 1 ? std::nullopt : std::optional<Distance>{0});
        }
        grilla.push_back(std::move(nueva_fila));
    }
    return grilla;
}

// Traduce la grilla de 1926 (char: '.'=libre, '+'=bloqueado) a la convención interna.
Grid traducir(const std::vector<std::vector<char>>& entrada) {
    Grid grilla;
    grilla.reserve(entrada.size());
    for (const auto& fila : entrada) {
        std::vector<Cell> nueva_fila;
        nueva_fila.reserve(fila.size());
        for (const char valor : fila) {
            nueva_fila.push_back(valor == '+' ? std::nullopt : std::optional<Distance>{0});
        }
        grilla.push_back(std::move(nueva_fila));
    }
    return grilla;
}

// BFS con contagio: siembra distancia 0 en el inicio y propaga con los offsets
// indicados. Devuelve la distancia del primer vecino que cumple `es_meta`
// (o del inicio mismo si lo cumple), o nullopt si no hay camino.
// `es_meta` es donde cada problema define su condición de éxito.
template <std::size_t N, typename EsMeta>
std::optional<Distance> bfs(Grid& grilla, const Position inicio,
                            const std::array<Position, N>& offsets, EsMeta es_meta) {
    if (!existe(grilla, inicio.first, inicio.second) ||
        !grilla[inicio.first][inicio.second].has_value()) {
        return std::nullopt;
    }
    grilla[inicio.first][inicio.second] = 0;
    if (es_meta(inicio)) {
        return 0;
    }
    std::queue<Position> cola;
    cola.push(inicio);
    while (!cola.empty()) {
        const Position actual = cola.front();
        cola.pop();
        for (const auto& vecino : vecinos_transitables(grilla, actual, offsets)) {
            const Distance distancia = *grilla[actual.first][actual.second] + 1;
            grilla[vecino.first][vecino.second] = distancia;
            cola.push(vecino);
            if (es_meta(vecino)) {
                return distancia;
            }
        }
    }
    return std::nullopt;
}

}  // namespace

// ==================== LeetCode 1926: Nearest Exit from Entrance in Maze ====================
class Solution {
public:
    int nearestExit(std::vector<std::vector<char>>& maze, std::vector<int>& entrance) {
        Grid grilla = traducir(maze);
        const Position inicio{static_cast<std::size_t>(entrance[0]),
                              static_cast<std::size_t>(entrance[1])};
        // 1926: cualquier borde distinto de la entrada, 4 direcciones. Cuenta pasos.
        const auto resultado = bfs(grilla, inicio, kOffsets4,
                                   [&grilla, inicio](const Position p) {
                                       return es_borde(grilla, p) && p != inicio;
                                   });
        return resultado.has_value() ? static_cast<int>(*resultado) : -1;
    }
};

// ==================== LeetCode 1091: Shortest Path in Binary Matrix ====================
/*
class Solution {
public:
    int shortestPathBinaryMatrix(std::vector<std::vector<int>>& grid) {
        Grid grilla = traducir(grid);
        const std::size_t n = grilla.size();
        const Position destino{n - 1, n - 1};
        // 1091: destino fijo, 8 direcciones. El resultado cuenta celdas: pasos + 1.
        const auto resultado = bfs(grilla, {0, 0}, kOffsets8,
                                   [destino](const Position p) { return p == destino; });
        return resultado.has_value() ? static_cast<int>(*resultado + 1) : -1;
    }
};
*/
