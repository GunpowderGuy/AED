// Compilar: g++ -std=c++23 -O2 -o bfs bfs.cpp
// BFS en C++23: dist en vector (mutación directa) y cola queue.
// Mismo grafo e invariante que los .idr: marcar AL ENCOLAR.
#include <bits/stdc++.h>
using namespace std;

int main() {
    // Ciclo 0-1-2-3-0
    vector<vector<int>> g = {{1, 3}, {0, 2}, {1, 3}, {2, 0}};

    const int INF = -1;  // -1 = no alcanzado (el "Nothing" de Idris)
    vector<int> dist(g.size(), INF);
    queue<int> q;

    dist[0] = 0;
    q.push(0);
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

    for (int d : dist) cout << d << ' ';
    cout << '\n';
}
