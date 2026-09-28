#include <iostream>
#include <vector>
#include <functional>
#include <concepts>


template<typename T, std::predicate<const T&, const T&> Compare>
bool esHeapValido(const std::vector<T>& v, const Compare comp){
    for (size_t i = 1; i < v.size(); ++i) {
        size_t padre = (i - 1) / 2;
        if (comp(v[i], v[padre])) return false;  // hijo "menor" que padre = violación
    }
    return true;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int T;
    std::cin >> T;
    while (T--) {
        int n;
        std::cin >> n;
        std::vector<int> H(n);
        for (auto& x : H) std::cin >> x;

        std::cout << (esHeapValido(H, std::less<int>{}) ? "SI" : "NO") << '\n';
          }
    return 0;
}


// Restaura la propiedad del heap hacia arriba desde la posicion i.
// El elemento en v[i] "flota" hasta que su padre lo ordene correctamente.
template<typename T, std::predicate<const T&, const T&> Compare>
void subir(std::vector<T>& v, size_t i, const Compare comp) {
    while (i > 0) {
        size_t padre = (i - 1) / 2;
        if (comp(v[padre], v[i])) break;  // ya ordenado respecto al padre
        std::swap(v[padre], v[i]);
        i = padre;
    }
}

// Inserta x en el heap manteniendo la propiedad: O(log n)
template<typename T, std::predicate<const T&, const T&> Compare>
void insertar(std::vector<T>& v, const T& x, const Compare comp) {
    v.push_back(x);
    subir(v, v.size() - 1, comp);
}
