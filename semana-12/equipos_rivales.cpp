/*

  Problema: equipos rivales (Codeforces, grupo nylekuuZuw, concurso 727947)
 
  Hay que repartir n estudiantes en dos equipos (rojo y azul, distintos)
  de modo que cada pareja de rivales quede en equipos distintos. Contar el
  numero de repartos validos modulo 10.pow(9)+7, o decir que es imposible.
 
  Idea: el grafo de rivalidades debe ser bipartito (2-coloreable). Se recorre
  componente por componente pintando con dos colores (BFS): si un vecino ya
  tiene el color del nodo actual, hay un ciclo impar y la respuesta es NO.
  Si todo colorea bien, cada componente conexa tiene exactamente 2 repartos
  (elegir que lado va al rojo), asi que el total es
  2.pow(#componentes conexas) mod 10.pow(9)+7. Los vertices aislados cuentan
  como componentes.
 */

#include <cstdint>
#include <iostream>
#include <queue>
#include <vector>

namespace {

constexpr std::int64_t kModulo = 1'000'000'007;

using Grafo = std::vector<std::vector<int>>;

// Potencia rapida: base.pow(exp) mod kModulo.
std::int64_t potencia_mod(std::int64_t base, std::int64_t exp) {
    std::int64_t resultado = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            resultado = resultado * base % kModulo;
        }
        base = base * base % kModulo;
        exp /= 2;
    }
    return resultado;
}

// Colorea la componente de `inicio` con BFS. Devuelve false si encuentra un
// conflicto (el grafo no es bipartito).
bool colorear_componente(const Grafo& grafo, const int inicio, std::vector<int>& color) {
    std::queue<int> cola;
    color[inicio] = 0;
    cola.push(inicio);

    while (!cola.empty()) {
        const int actual = cola.front();
        cola.pop();
        for (const int vecino : grafo[actual]) {
            if (color[vecino] == color[actual]) {
                return false;  // rival en el mismo equipo
            }
            if (color[vecino] == -1) {
                color[vecino] = 1 - color[actual];
                cola.push(vecino);
            }
        }
    }
    return true;
}

// Devuelve el numero de repartos mod kModulo, o -1 si es imposible.
std::int64_t repartos_validos(const Grafo& grafo) {
    const int n = static_cast<int>(grafo.size());
    std::vector<int> color(n, -1);  // -1 = sin colorear
    std::int64_t componentes = 0;

    for (int i = 0; i < n; ++i) {
        if (color[i] != -1) {
            continue;
        }
        componentes++;
        if (!colorear_componente(grafo, i, color)) {
            return -1;
        }
    }
    return potencia_mod(2, componentes);
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);

    int n, m;
    std::cin >> n >> m;

    Grafo grafo(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;
        grafo[u - 1].push_back(v - 1);  // a 0-indexado; las repetidas no molestan
        grafo[v - 1].push_back(u - 1);
    }

    const std::int64_t resultado = repartos_validos(grafo);
    if (resultado == -1) {
        std::cout << "NO\n";
    } else {
        std::cout << "SI " << resultado << '\n';
    }
    return 0;
}
