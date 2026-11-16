// Compilar: g++ -std=c++23 -O2 -o bfs bfs.cpp
// BFS en C++23: dist en vector (mutación directa) y cola queue.
// Mismo grafo e invariante que los .idr: marcar AL ENCOLAR.
#include <bits/stdc++.h>
using namespace std;

// BFS desde `origen`: devuelve las distancias (INF = no alcanzado, el
// "Nothing" de Idris). Con while, como el C++ es: la cola termina sola.
vector<int> bfs(const vector<vector<int>>& g, int origen) {
    const int INF = -1;
    vector<int> dist(g.size(), INF);
    queue<int> q;

    dist[origen] = 0;
    q.push(origen);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : g[u]) {
            if (dist[v] == INF) {  // marcar AL ENCOLAR
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    return dist;
}

// Igual que bfs, pero el bucle es un for acotado por la cota O(V + E)
// del BFS: como cada nodo se encola ≤ 1 vez, el número de iteraciones es
// ≤ 1 + E ≤ V + E, así que el for nunca corta por fuel, siempre por cola
// vacía (el "break"). Es el while de arriba con el fuel de los .idr.
vector<int> bfsFor(const vector<vector<int>>& g, int origen) {
    const int INF = -1;
    vector<int> dist(g.size(), INF);
    queue<int> q;

    dist[origen] = 0;
    q.push(origen);

    int aristas = 0;
    for (const auto& vs : g) {
        aristas += (int)vs.size();
    }
    int fuel = (int)g.size() + aristas;  // O(V + E): ≥ máx. de iteraciones

    for (int i = 0; i < fuel && !q.empty(); ++i) {
        int u = q.front();
        q.pop();
        for (int v : g[u]) {
            if (dist[v] == INF) {  // marcar AL ENCOLAR
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    return dist;
}

int main() {
    // Ciclo 0-1-2-3-0
    vector<vector<int>> g = {{1, 3}, {0, 2}, {1, 3}, {2, 0}};

    for (int d : bfs(g, 0)) {
        cout << d << ' ';
    }
    cout << '\n';
    for (int d : bfsFor(g, 0)) {
        cout << d << ' ';
    }
    cout << '\n';
}
