#include <cstddef>
#include <iostream>
#include "AVL.hpp"

using namespace std;

// Limite del enunciado: 0 <= x <= 200000.
constexpr size_t MAX_X = 200000;

// Registro de camisetas: conteo de jugadores por numero, mas el conjunto
// de numeros actualmente libres (0..MAX_X).
class RegistroCamisetas {
public:
    RegistroCamisetas() : conteo_(MAX_X + 1, 0) {
        for (size_t x = 0; x <= MAX_X; ++x)
            libres_.insert(x);
    }

    // Entra un jugador con la camiseta x.
    void entra(size_t x) {
        if (conteo_[x] == 0)
            libres_.erase(x);
        ++conteo_[x];
    }

    // Sale un jugador con la camiseta x (garantizado: habia al menos uno).
    void sale(size_t x) {
        --conteo_[x];
        if (conteo_[x] == 0)
            libres_.insert(x);
    }

    // Menor numero de camiseta que no esta en uso.
    // Si todos los 0..MAX_X estan ocupados, la respuesta es MAX_X + 1
    // (ese numero nunca se registra, asi que jamas desaparece del conjunto
    // de libres: lo devolvemos como sentinela).
    size_t menor_libre() {
        if (libres_.empty())
            return MAX_X + 1;
        return libres_.min_element()->data;
    }

private:
    vector<size_t> conteo_;
    AVL<size_t> libres_;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    size_t q;
    cin >> q;
    RegistroCamisetas registro;
    while (q--) {
        size_t tipo;
        cin >> tipo;
        if (tipo == 1) {
            size_t x;
            cin >> x;
            registro.entra(x);
        } else if (tipo == 2) {
            size_t x;
            cin >> x;
            registro.sale(x);
        } else {
            cout << registro.menor_libre() << '\n';
        }
    }
    return 0;
}
