/*

Cuadrado Magico : Es una cuadricula de numeros en la que todas las columnas y las dos diagonales principales suman el mismo valor


Es una cuadricula de numeros en la que todas las columnas y las dos diagonales principales suman el mismo valor . Este valor se llama constante magica 


como en este problema los valores de cada casilla pueden ser de 1 a 9. entonces la manera mas simplista posible de resolverlo, es :

Generar todos los numeros posibles para los espacios vacios . Luego revisar cual es un cuadrado magico.

*/

#include <array>
#include <vector>

#include <iostream>

using std::size_t;   // se puede escribir size_t sin std::		

bool OLDesMagico(int m[3][3]) {
    // Las 3 filas y las 3 columnas
    for (int i = 0; i < 3; i++) {
        if (m[i][0] + m[i][1] + m[i][2] != 15) return false; // fila i
        if (m[0][i] + m[1][i] + m[2][i] != 15) return false; // columna i
    }
    // Las 2 diagonales
    if (m[0][0] + m[1][1] + m[2][2] != 15) return false; // principal
    if (m[0][2] + m[1][1] + m[2][0] != 15) return false; // secundaria

    return true;
}




bool esMagico(const std::array<std::array<std::size_t,3>,3> m){

const std::size_t magica = 15;

// Las 3 filas y las 3 columnas
    for (int i = 0; i < 3; i++) {
        if (m[i][0] + m[i][1] + m[i][2] != 15) return false; // fila i
        if (m[0][i] + m[1][i] + m[2][i] != 15) return false; // columna i
    }

// las dos diagonales
if(m[0][0] + m[1][1] + m[2][2] != magica) return false;
if(m[0][2] + m[1][1] + m[2][0] != magica) return false;

return true;
}


std::size_t contarHuecos(const std::array<std::array<std::size_t, 3>, 3>& cuadrado) {
    std::size_t contador = 0;
    for (std::size_t i = 0; i < 3; i++){
        for (std::size_t j = 0; j < 3; j++){
            if (cuadrado[i][j] == 0) { contador++; }
            }
          }
    return contador;
}

// Funcion auxiliar (recursiva): llena la posicion k del vector "actual"
void generarAux(const size_t k, const size_t n, std::vector<size_t>& actual,
                std::vector<std::vector<size_t>>& resultado) {
    if (k == n) {                     // el vector está completo
        resultado.push_back(actual);
        return;
    }
    for (size_t v = 1; v <= 9; v++) {    // este hueco prueba 1..9
        actual[k] = v;
        generarAux(k + 1, n, actual, resultado);  // siguiente hueco
    }
} // Esa funcion solo deberia ser llamada por todosLosVectores

// Funcion publica: le das n, te devuelve todos los vectores
std::vector<std::vector<size_t>> todosLosVectores(const size_t n) {
    std::vector<std::vector<size_t>> resultado;
    std::vector<size_t> actual(n);
    generarAux(0, n, actual, resultado);
    return resultado;
}

// Toma un cuadrado con casillas vacías (0) y devuelve TODAS las formas
// de rellenarlas con dígitos del 1 al 9, sin filtrar nada.
std::vector<std::array<std::array<std::size_t, 3>, 3>> completaciones(
    const std::array<std::array<std::size_t, 3>, 3>& incompleto) {

    const std::size_t n = contarHuecos(incompleto);
    std::vector<std::array<std::array<std::size_t, 3>, 3>> resultado;

    for (const auto& relleno : todosLosVectores(n)) {
        std::array<std::array<std::size_t, 3>, 3> candidato = incompleto;
        std::size_t k = 0;
        for (std::size_t i = 0; i < 3; i++)
            for (std::size_t j = 0; j < 3; j++)
                if (candidato[i][j] == 0)
                    candidato[i][j] = relleno[k++];
        resultado.push_back(candidato);
    }
    return resultado;
}


// Deja pasar solo los cuadrados que SI son magicos
std::vector<std::array<std::array<std::size_t, 3>, 3>> filtrarMagicos(
    const std::vector<std::array<std::array<std::size_t, 3>, 3>>& cuadrados) {

    std::vector<std::array<std::array<std::size_t, 3>, 3>> resultado;
    for (const auto& c : cuadrados)
        if (esMagico(c))
            resultado.push_back(c);
    return resultado;
}

// Verdadero cuadrado magico: (1) las 8 lineas suman 15 Y
// (2) los digitos 1-9 aparecen exactamente una vez cada uno
bool CorrectoEsMagico(const std::array<std::array<std::size_t, 3>, 3>& m) {
    const std::size_t magica = 15;

    // (1) las 8 lineas
    for (std::size_t i = 0; i < 3; i++) {
        if (m[i][0] + m[i][1] + m[i][2] != magica) return false; // fila i
        if (m[0][i] + m[1][i] + m[2][i] != magica) return false; // columna i
    }
    if (m[0][0] + m[1][1] + m[2][2] != magica) return false;     // diagonal principal
    if (m[0][2] + m[1][1] + m[2][0] != magica) return false;     // diagonal secundaria

    // (2) cada digito del 1 al 9, exactamente una vez
    bool visto[10] = {};   // visto[d] = true si el digito d ya aparecio
    for (std::size_t i = 0; i < 3; i++)
        for (std::size_t j = 0; j < 3; j++) {
            std::size_t d = m[i][j];
            if (d < 1 || d > 9 || visto[d]) return false;
            visto[d] = true;
        }
    return true;
}


// Deja pasar solo los cuadrados que SI son magicos (verificacion completa)
std::vector<std::array<std::array<std::size_t, 3>, 3>> CorrectoFiltrarMagicos(
    const std::vector<std::array<std::array<std::size_t, 3>, 3>>& cuadrados) {

    std::vector<std::array<std::array<std::size_t, 3>, 3>> resultado;
    for (const auto& c : cuadrados)
        if (CorrectoEsMagico(c))
            resultado.push_back(c);
    return resultado;
}


// Genera TODOS los cuadrados magicos completos a partir de uno incompleto
std::vector<std::array<std::array<std::size_t, 3>, 3>> cuadradosMagicos(
    const std::array<std::array<std::size_t, 3>, 3>& incompleto) {

    return CorrectoFiltrarMagicos(completaciones(incompleto));
}

using Grid = std::array<std::array<size_t, 3>, 3>;

int main() {
    Grid g;
    for (size_t i = 0; i < 3; i++)
        for (size_t j = 0; j < 3; j++)
            std::cin >> g[i][j];

    std::cout << cuadradosMagicos(g).size() << '\n';
    return 0;
}

//int main (){
//return 0;
//}

