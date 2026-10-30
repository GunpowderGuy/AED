// Robot en laberinto (Codeforces, problema D del grupo)
//
// El robot empieza en (1,1) mirando a la derecha; cada acción
// (avanzar, girar izquierda, girar derecha) cuesta 1.
// Objetivo: llegar a (n,m) con cualquier dirección, con mínimas acciones.
//
// BFS sobre el grafo de estados (fila, columna, dirección): todas las
// aristas pesan 1, así que BFS clásico da la respuesta óptima.

#include <array>
#include <cstddef>
#include <iostream>
#include <optional>
#include <queue>
#include <string>
#include <utility>
#include <vector>

using Distance = std::size_t;  // 0 = sin visitar; >=1 = acciones acumuladas
using Laberinto = std::vector<std::string>;
using Casilla = std::pair<std::size_t, std::size_t>;

// Direcciones en orden horario: derecha, abajo, izquierda, arriba.
constexpr std::array<Casilla, 4> kDirecciones{{Casilla{0, 1}, Casilla{1, 0}, Casilla{0, static_cast<std::size_t>(-1)}, Casilla{static_cast<std::size_t>(-1), 0}}};

struct Estado {
    std::size_t fila;
    std::size_t columna;
    std::size_t direccion;  // índice en kDirecciones
};

// distancias[fila][columna][direccion]
using Distancias = std::vector<std::vector<std::array<Distance, 4>>>;

bool existe(const Laberinto& laberinto, const std::size_t fila, const std::size_t columna) {
    return fila < laberinto.size() && columna < laberinto[fila].size();
}

bool es_libre(const Laberinto& laberinto, const std::size_t fila, const std::size_t columna) {
    return existe(laberinto, fila, columna) && laberinto[fila][columna] == '.';
}

// Giro de 90°: -1 izquierda, +1 derecha (el orden de kDirecciones es horario).
std::size_t girar(const std::size_t direccion, const int sentido) {
    return static_cast<std::size_t>((static_cast<int>(direccion) + sentido + 4) % 4);
}

// Encola el estado vecino si no fue visitado y devuelve true si se encoló.
bool visitar(Distancias& distancias, std::queue<Estado>& cola, const Estado vecino,
             const Distance distancia) {
    if (distancias[vecino.fila][vecino.columna][vecino.direccion] != 0) {
        return false;
    }
    distancias[vecino.fila][vecino.columna][vecino.direccion] = distancia;
    cola.push(vecino);
    return true;
}

// Genera los estados alcanzables en 1 acción desde `actual`: dos giros en
// el mismo lugar y, si la casilla vecina es libre, avanzar.
std::vector<Estado> acciones_posibles(const Laberinto& laberinto, const Estado actual) {
    const auto [fila, columna, direccion] = actual;
    std::vector<Estado> resultado;
    resultado.reserve(3);
    resultado.push_back({fila, columna, girar(direccion, -1)});
    resultado.push_back({fila, columna, girar(direccion, +1)});
    const auto [df, dc] = kDirecciones[direccion];
    const std::size_t nf = fila + df;
    const std::size_t nc = columna + dc;
    if (es_libre(laberinto, nf, nc)) {
        resultado.push_back({nf, nc, direccion});
    }
    return resultado;
}

// BFS por contagio sobre estados. Devuelve las acciones mínimas para llegar
// a `destino` con cualquier dirección, o nullopt si es imposible.
std::optional<Distance> acciones_minimas(const Laberinto& laberinto, const Estado inicio,
                                       const Casilla destino) {
    Distancias distancias(laberinto.size());
    for (std::size_t i = 0; i < laberinto.size(); ++i) {
        distancias[i].assign(laberinto[i].size(), {0, 0, 0, 0});
    }
    distancias[inicio.fila][inicio.columna][inicio.direccion] = 0;
    if (Casilla{inicio.fila, inicio.columna} == destino) {
        return 0;
    }

    std::queue<Estado> cola;
    cola.push(inicio);
    while (!cola.empty()) {
        const Estado actual = cola.front();
        cola.pop();
        const Distance siguiente = distancias[actual.fila][actual.columna][actual.direccion] + 1;
        for (const Estado& vecino : acciones_posibles(laberinto, actual)) {
            if (visitar(distancias, cola, vecino, siguiente) &&
                Casilla{vecino.fila, vecino.columna} == destino) {
                return siguiente;
            }
        }
    }
    return std::nullopt;
}

int main() {
    std::size_t n{}, m{};
    std::cin >> n >> m;

    Laberinto laberinto;
    laberinto.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        std::string fila;
        std::cin >> fila;
        laberinto.push_back(std::move(fila));
    }

    // El robot arranca en (1,1) (0,0 en 0-indexado) mirando a la derecha.
    const Estado inicio{0, 0, 0};
    const Casilla destino{n - 1, m - 1};

    const auto resultado = acciones_minimas(laberinto, inicio, destino);
    std::cout << (resultado.has_value() ? static_cast<long long>(*resultado) : -1) << '\n';
    
    return 0;
}
