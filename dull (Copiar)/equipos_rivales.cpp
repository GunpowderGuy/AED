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


 /*
B. Visita obligada
time limit per test
2 seconds
memory limit per test
256 megabytes

En el barrio de Benjamin hay n esquinas, numeradas de 1 a n, y m calles. Cada calle une dos esquinas distintas, se puede recorrer en ambos sentidos, y no hay dos calles entre el mismo par de esquinas. Benjamin vive en la esquina 1 y su colegio está en la esquina n.

Hoy debe pasar por la panadería, que está en la esquina x, antes de ir al colegio. Calcula la menor cantidad de calles que debe recorrer para ir de su casa al colegio pasando por la esquina x (puede pasar por otras esquinas, e incluso repetir calles, si le conviene).
Input

La primera línea contiene tres enteros n, m y x (1≤n≤2⋅105, 0≤m≤min(2⋅105,n(n−1)/2), 1≤x≤n).

Cada una de las siguientes m líneas contiene dos enteros u y v (1≤u,v≤n, u≠v): hay una calle entre las esquinas u y v. No hay pares repetidos.
Output

Imprime la menor cantidad de calles, o −1 si no es posible ir de la casa al colegio pasando por la panadería.
Scoring

Límites

Este problema tiene subtareas, por lo cual es posible obtener un puntaje en base a las subtareas que logren ser resueltas.

    1≤n≤2⋅105
    0≤m≤2⋅105

Grupo 1 (1 punto)

    n≤6

Grupo 2 (1 punto)

    n≤1000

Grupo 3 (2 puntos)

    Sin restricciones adicionales.

*/

#include <iostream>
#include <queue>
#include <vector>

namespace {

using Grafo = std::vector<std::vector<int>>;

// BFS: distancia (en calles) desde `inicio` hasta cada esquina.
// -1 = inalcanzable. El grafo no tiene pesos, asi que BFS da el camino minimo.
std::vector<int> distancias_desde(const Grafo& grafo, const int inicio) {
    std::vector<int> distancia(grafo.size(), -1);
    std::queue<int> cola;
    distancia[inicio] = 0;
    cola.push(inicio);

    while (!cola.empty()) {
        const int actual = cola.front();
        cola.pop();
        for (const int vecino : grafo[actual]) {
            if (distancia[vecino] == -1) {
                distancia[vecino] = distancia[actual] + 1;
                cola.push(vecino);
            }
        }
    }
    return distancia;
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m, x;
    std::cin >> n >> m >> x;

    Grafo grafo(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;
        grafo[u - 1].push_back(v - 1);  // a 0-indexado
        grafo[v - 1].push_back(u - 1);
    }

    // El recorrido 1 -> x -> n minimo es dist(1, x) + dist(x, n):
    // cualquier paseo que pase por x tiene esa longitud como cota inferior,
    // y unir los dos caminos minimos ya da un paseo que la cumple (aunque
    // se repitan calles).
    const std::vector<int> desde_casa = distancias_desde(grafo, 0);        // esquina 1
    const std::vector<int> desde_colegio = distancias_desde(grafo, n - 1);  // esquina n

    const int ida = desde_casa[x - 1];
    const int regreso = desde_colegio[x - 1];
    if (ida == -1 || regreso == -1) {
        std::cout << -1 << '\n';
    } else {
        std::cout << ida + regreso << '\n';
    }
    return 0;
}
