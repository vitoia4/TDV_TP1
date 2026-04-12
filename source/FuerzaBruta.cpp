#include "FuerzaBruta.h"

#include <iostream>
#include <vector>
#include <limits>


void FB(const std::vector<std::vector<double>>& energia, int fila, std::vector<int> & mejor, std::vector<int> actual, double &mejorEnergia, double energiaActual) {
    int n = energia[0][0];
    int m = energia[0][1];
    
    if(fila == n+1){
        if(energiaActual < mejorEnergia){
            mejor = actual;
            mejorEnergia = energiaActual;
        }
        return;
    }

    if(fila==1){
        for (int col=0; col < m; col++){
            actual.push_back(col);
            FB(energia, fila+1, mejor, actual, mejorEnergia, energiaActual + energia[fila][col]);
            actual.pop_back();
        }
    }

    else{
        int ult_columna = actual.back();

        for (int col = ult_columna - 1; col <= ult_columna + 1; col++) {
            if (col >= 0 && col < m) { // poda de factibilidad
                actual.push_back(col);
                FB(energia, fila + 1, mejor, actual, mejorEnergia, energiaActual + energia[fila][col]);
                actual.pop_back();
            }
        }
    }

}




std::vector<int> encontrarSeamFuerzaBruta(const std::vector<std::vector<double>>& energia) {
    int n = energia[0][0];
    int m = energia[0][1];

    double mejorEnergia = std::numeric_limits<double>::infinity();

    std::vector<int> mejor;
    std::vector<int> actual =  {};
    FB(energia, 1, mejor, actual, mejorEnergia, 0);

    //Para que sea indexado 1
    for(int i=0; i<n; i++){
        mejor[i]++;
    }

    return mejor;
}

//------------------------------------------

double calcularEnergiaSeam(const std::vector<std::vector<double>>& energiaMatriz, const std::vector<int>& seam) {
    double total = 0.0;
    for (int i = 0; i < seam.size(); i++) {
        total += energiaMatriz[i+1][seam[i]];
    }
    return total;
}

// === FUNCION AUXILIAR PARA TESTEAR ===
void testCaso(const std::vector<std::vector<double>>& energia, const std::string& nombre) {
    std::cout << "==== " << nombre << " ====" << std::endl;

    std::vector<int> seam = encontrarSeamFuerzaBruta(energia);

    std::cout << "Seam (1-indexado): ";
    for (int c : seam) {
        std::cout << c << " ";
    }
    std::cout << std::endl;

    std::cout << "Energia: "
              << calcularEnergiaSeam(energia, seam)
              << std::endl;

    std::cout << std::endl;
}

// === MAIN ===
int main() {

    // 🧪 Caso 1
    std::vector<std::vector<double>> caso1 = {
        {3, 3},
        {1, 2, 3},
        {4, 1, 6},
        {7, 8, 1}
    };

    // 🧪 Caso 2
    std::vector<std::vector<double>> caso2 = {
        {4, 4},
        {10, 1, 10, 10},
        {10, 1, 10, 10},
        {10, 1, 10, 10},
        {10, 1, 10, 10}
    };

    // 🧪 Caso 3
    std::vector<std::vector<double>> caso3 = {
        {5, 5},
        {3, 4, 1, 2, 8},
        {6, 1, 8, 2, 7},
        {5, 9, 3, 9, 9},
        {8, 4, 1, 3, 2},
        {3, 7, 2, 8, 6}
    };

    // 🧪 Tu caso
    std::vector<std::vector<double>> caso4 = {
        {5, 6},
        {9.0, 3.0, 1.0, 2.0, 8.0, 7.0},
        {5.0, 2.0, 0.5, 4.0, 6.0, 3.0},
        {7.0, 1.0, 2.0, 0.8, 5.0, 4.0},
        {3.0, 4.0, 1.5, 1.0, 2.0, 6.0},
        {8.0, 2.0, 3.0, 0.5, 1.0, 5.0}
    };

    // === EJECUTAR TESTS ===
    testCaso(caso1, "Caso 1");
    testCaso(caso2, "Caso 2");
    testCaso(caso3, "Caso 3");
    testCaso(caso4, "Caso Enunciado");

    return 0;
}
