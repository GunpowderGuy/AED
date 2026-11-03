
#include <iostream>
#include <queue>
#include <vector>

namespace {

using Grafo = std::vector<std::vector<int>>;


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

}  

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m, x;
    std::cin >> n >> m >> x;

    Grafo grafo(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;
        grafo[u - 1].push_back(v - 1);  
        grafo[v - 1].push_back(u - 1);
    }

   
    const std::vector<int> desde_casa = distancias_desde(grafo, 0);       
    const std::vector<int> desde_colegio = distancias_desde(grafo, n - 1); 

    const int ida = desde_casa[x - 1];
    const int regreso = desde_colegio[x - 1];
    if (ida == -1 || regreso == -1) {
        std::cout << -1 << '\n';
    } else {
        std::cout << ida + regreso << '\n';
    }
    return 0;
}
