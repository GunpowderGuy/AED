/*

  Problema: relaciones de mando (Codeforces, grupo nylekuuZuw, concurso 728509)
 
  Hay n personas y m relaciones (u, v): entre u y v hay relacion jefe-
  subordinado, pero no se anoto quien manda a quien. Pueden repetirse pares
  (son relaciones distintas) y puede aparecer (u, u), que significa que u es
  su propio jefe. Hay que decidir para cada relacion quien es el subordinado,
  de modo que cada persona sea subordinado en a lo mas una relacion.
  Imprimir SI si se puede, NO en caso contrario.
 
  Idea: modelar el grafo (multigrafo, con lazos) y preguntarse si se pueden
  orientar sus aristas de modo que cada vertice tenga a lo mas una arista
  entrante (ser subordinado = recibir la arista). Cada arista necesita un
  "cabeza" distinta por vertice, asi que un componente con e aristas y v
  vertices solo puede orientarse si e <= v. Esa condicion es tambien
  suficiente: un componente asi es un arbol o tiene un solo ciclo; el arbol
  se orienta desde la raiz, y con un ciclo se orienta el ciclo en sentido
  unico y lo colgado hacia adentro.
 
  Algoritmo: Union-Find. Se unen los extremos de cada relacion y se cuenta
  cuantas aristas caen en cada componente. Al final, SI si en todo
  componente #aristas <= #vertices, NO si no.
 */

#include <iostream>
#include <numeric>
#include <vector>

namespace {

// Union-Find: padre[i] y tam[i] (tamano del componente).
class UnionFind {
public:
    explicit UnionFind(const int n) : padre_(n), tam_(n, 1) {
        std::iota(padre_.begin(), padre_.end(), 0);
    }

    int raiz(const int x) {
        if (padre_[x] != x) {
            padre_[x] = raiz(padre_[x]);  // compresion de caminos
        }
        return padre_[x];
    }

    void unir(const int a, const int b) {
        int ra = raiz(a);
        int rb = raiz(b);
        if (ra == rb) {
            return;
        }
        if (tam_[ra] < tam_[rb]) {
            std::swap(ra, rb);
        }
        padre_[rb] = ra;  // union por tamano
        tam_[ra] += tam_[rb];
    }

    int tamano(const int x) { return tam_[raiz(x)]; }

private:
    std::vector<int> padre_;
    std::vector<int> tam_;
};

// Decide si se puede asignar subordinados: cada componente conexa debe tener
// a lo mas tantas relaciones como personas.
bool es_posible(const int n, const std::vector<std::pair<int, int>>& relaciones) {
    UnionFind uf(n);
    for (const auto& [u, v] : relaciones) {
        uf.unir(u, v);  // los lazos (u, u) no unen nada, pero cuentan abajo
    }

    std::vector<int> aristas(n, 0);
    for (const auto& [u, v] : relaciones) {
        aristas[uf.raiz(u)]++;
    }

    for (int i = 0; i < n; ++i) {
        if (uf.raiz(i) == i && aristas[i] > uf.tamano(i)) {
            return false;
        }
    }
    return true;
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);

    int n, m;
    std::cin >> n >> m;

    std::vector<std::pair<int, int>> relaciones;
    relaciones.reserve(m);
    for (int i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;
        relaciones.emplace_back(u - 1, v - 1);  // a 0-indexado
    }

    std::cout << (es_posible(n, relaciones) ? "SI" : "NO") << '\n';
    return 0;
}
