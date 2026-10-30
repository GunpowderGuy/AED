/*
  Problema: cerradura de ruedas (Codeforces, grupo nylekuuZuw, concurso 728509)
 
  Una cerradura tiene w ruedas (1 <= w <= 4), cada una con digitos 0-9.
  En un movimiento se gira una sola rueda un lugar: el digito pasa de d a
  d+1 (subir) o d-1 (bajar), con vuelta completa (9 -> 0 y 0 -> 9).
  Hay k codigos prohibidos: no se puede PASAR por uno de ellos (ni empezar
  ni terminar ahi). Dados el codigo inicial s y el objetivo t, hallar la
  minima cantidad de movimientos para llegar de s a t, o -1 si es imposible.
 
  Idea: BFS sobre el espacio de estados = los 10^w codigos posibles.
  El estado es un codigo; sus vecinos son los codigos que se obtienen
  girando exactamente una rueda un lugar (2*w vecinos como maximo, todos
  de costo 1). Los codigos prohibidos actuan como obstaculos: se ignoran
  al expandir. Como todas las aristas pesan 1, BFS clasico da el minimo.
 
  Representacion: cada codigo se codifica como un entero 0..10^w - 1
  (su valor numerico), lo que permite un arreglo plano de distancias.
 */

#include <cstddef>
#include <iostream>
#include <optional>
#include <queue>
#include <string>
#include <vector>

namespace {

using Codigo = int;  // codigo numerico en [0, 10.pow(w))

// Convierte una cadena de w digitos a su valor numerico.
Codigo codificar(const std::string& texto) {
    Codigo valor = 0;
    for (const char c : texto) {
        valor = valor * 10 + (c - '0');
    }
    return valor;
}

// Genera los vecinos de `codigo`: girar cada rueda un lugar arriba o abajo.
// `potencias[i]` es el valor posicional de la rueda i (10.pow(w-1-i)).
std::vector<Codigo> vecinos(const Codigo codigo, const std::vector<Codigo>& potencias) {
    std::vector<Codigo> resultado;
    resultado.reserve(2 * potencias.size());
    for (const Codigo pot : potencias) {
        const int digito = (codigo / pot) % 10;
        // Subir: digito+1, con vuelta 9 -> 0.
        const int arriba = (digito == 9) ? codigo - 9 * pot : codigo + pot;
        // Bajar: digito-1, con vuelta 0 -> 9.
        const int abajo = (digito == 0) ? codigo + 9 * pot : codigo - pot;
        resultado.push_back(arriba);
        resultado.push_back(abajo);
    }
    return resultado;
}

// BFS por contagio: devuelve los movimientos minimos de inicio a destino,
// o nullopt si no hay camino que evite los prohibidos.
std::optional<int> movimientos_minimos(const Codigo inicio, const Codigo destino,
                                       const std::vector<bool>& prohibido,
                                       const std::vector<Codigo>& potencias,
                                       const int total_codigos) {
    if (prohibido[inicio] || prohibido[destino]) {
        return std::nullopt;
    }
    std::vector<int> distancia(total_codigos, -1);  // -1 = sin visitar
    std::queue<Codigo> cola;
    distancia[inicio] = 0;
    cola.push(inicio);

    while (!cola.empty()) {
        const Codigo actual = cola.front();
        cola.pop();
        if (actual == destino) {
            return distancia[actual];
        }
        for (const Codigo vecino : vecinos(actual, potencias)) {
            if (distancia[vecino] != -1 || prohibido[vecino]) {
                continue;
            }
            distancia[vecino] = distancia[actual] + 1;
            cola.push(vecino);
        }
    }
    return std::nullopt;
}

}  // namespace

int main() {
    std::size_t w{};
    std::cin >> w;

    std::string s, t;
    std::cin >> s >> t;

    // 10^w codigos posibles; potencias de cada rueda (10.pow(0) .. 10.pow(w-1)).
    int total = 1;
    std::vector<Codigo> potencias;
    potencias.reserve(w);
    for (std::size_t i = 0; i < w; ++i) {
        potencias.push_back(total);
        total *= 10;  // al final: total = 10^w
    }

    std::size_t k{};
    std::cin >> k;
    std::vector<bool> prohibido(total, false);
    for (std::size_t i = 0; i < k; ++i) {
        std::string codigo;
        std::cin >> codigo;
        prohibido[codificar(codigo)] = true;
    }

    const auto resultado = movimientos_minimos(codificar(s), codificar(t), prohibido,
                                               potencias, total);
    std::cout << (resultado.has_value() ? *resultado : -1) << '\n';
    return 0;
}
