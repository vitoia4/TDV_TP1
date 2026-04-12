#include "Backtracking.h"
#include <cstddef>
#include <string>
#include <iostream>
#include <limits>


void BT(const std::vector<std::vector<double>>& energia, int fila, std::vector<int>&mejor, std::vector<int> actual, double & mejorEnergia, double energiaActual) {
    int n = energia[0][0];
    int m = energia[0][1];

    if(fila == n+1){
        if(energiaActual <= mejorEnergia){
            mejor = actual;
            mejorEnergia = energiaActual;
            
        }
        return;

    }

    if(fila==1){
        for (int col=0; col < energia[1].size(); col++){
            actual.push_back(col);
            BT(energia, fila+1, mejor, actual, mejorEnergia, energiaActual + energia[fila][col]);
            actual.pop_back();
            
            }
        }

    else{
        // Ṕoda: no seguir costura si energia actual > energia total mejor
        // Poda: recorrer solo casilleros válidos por definición de costura vertical
        if(energiaActual < mejorEnergia){
            int ult_columna = actual.back();

            for (int col = ult_columna - 1; col <= ult_columna + 1; col++) {
                if (col >= 0 && col < m) { // poda de factibilidad
                    actual.push_back(col);
                    BT(energia, fila + 1, mejor, actual, mejorEnergia, energiaActual+ energia[fila][col]);
                    actual.pop_back();
                }
            }
        }  
    }
    return;
}

std::vector<int> encontrarSeamBacktracking(const std::vector<std::vector<double>>& energia) {
    int n = energia[0][0];

    double mejorEnergia = std::numeric_limits<double>::infinity();

    std::vector<int> mejor;
    std::vector<int> actual =  {};

    BT(energia, 1, mejor, actual, mejorEnergia, 0);

    //Para que sea indexado 1
    for(int i=0; i<n; i++){
        mejor[i]++;
    }

    return mejor;
    
}

//------------------------------------------------------------------

double calcularEnergiaSeam(const std::vector<std::vector<double>>& energiaMatriz, const std::vector<int>& seam) {
    double total = 0.0;
    for (int i = 0; i < seam.size(); i++) {
        total += energiaMatriz[i+1][seam[i]];
    }
    return total;
}

// === FUNCION AUXILIAR ===
void testCaso(const std::vector<std::vector<double>>& energia,
              const std::string& nombre,
              const std::vector<int>& esperado) {

    std::cout << "==== " << nombre << " ====" << std::endl;

    std::vector<int> seam = encontrarSeamBacktracking(energia);

    std::cout << "Seam obtenido: ";
    for (int c : seam) std::cout << c << " ";
    std::cout << std::endl;

    if (!esperado.empty()) {
        std::cout << "Seam esperado: ";
        for (int c : esperado) std::cout << c << " ";
        std::cout << std::endl;
    } else {
        std::cout << "(Cualquier seam valido es correcto)" << std::endl;
    }

    std::cout << "Energia: "
              << calcularEnergiaSeam(energia, seam)
              << std::endl;

    std::cout << std::endl;
}

// === MAIN ===
int main() {

    // 🧪 Caso 1: borde (una sola fila)
    std::vector<std::vector<double>> caso1 = {
        {1, 5},
        {5, 1, 3, 2, 4}
    };
    testCaso(caso1, "Caso 1 (una fila)", {2});

    // 🧪 Caso 2: todos iguales
    std::vector<std::vector<double>> caso2 = {
        {4, 4},
        {1, 1, 1, 1},
        {1, 1, 1, 1},
        {1, 1, 1, 1},
        {1, 1, 1, 1}
    };
    testCaso(caso2, "Caso 2 (todos iguales)", {});

    // 🧪 Caso 3: trampa (columna fija óptima)
    std::vector<std::vector<double>> caso3 = {
        {4, 4},
        {1, 100, 100, 100},
        {1, 100, 100, 100},
        {1, 100, 100, 100},
        {1, 1,   1,   1}
    };
    testCaso(caso3, "Caso 3 (trampa columna)", {1, 1, 1, 1});

    // 🧪 Caso 4: zig-zag
    std::vector<std::vector<double>> caso4 = {
        {5, 5},
        {1, 100, 1, 100, 1},
        {100, 1, 100, 1, 100},
        {1, 100, 1, 100, 1},
        {100, 1, 100, 1, 100},
        {1, 100, 1, 100, 1}
    };
    testCaso(caso4, "Caso 4 (zig-zag)", {1, 2, 1, 2, 1});

    // 🧪 Caso 5: el del enunciado
    std::vector<std::vector<double>> caso5 = {
        {5, 6},
        {9.0, 3.0, 1.0, 2.0, 8.0, 7.0},
        {5.0, 2.0, 0.5, 4.0, 6.0, 3.0},
        {7.0, 1.0, 2.0, 0.8, 5.0, 4.0},
        {3.0, 4.0, 1.5, 1.0, 2.0, 6.0},
        {8.0, 2.0, 3.0, 1.5, 1.0, 5.0}
    };
    testCaso(caso5, "Caso 5 (enunciado)", {3, 3, 4, 4, 5});

    return 0;
}