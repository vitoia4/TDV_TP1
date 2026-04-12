#include "FuerzaBruta.h"
#include <iostream>
#include <vector>
#include <limits>


void FB(const std::vector<std::vector<double>>& energia, int fila, std::vector<int>& mejor, std::vector<int> actual, double& mejorEnergia, double energiaActual) {
    // CAMBIO: n y m se leen con .size() porque la matriz ya no tiene fila de dimensiones
    int n = energia.size();
    int m = energia[0].size();

    if (fila == n + 1) {
        if (energiaActual < mejorEnergia) {
            mejor = actual;
            mejorEnergia = energiaActual;
        }
        return;
    }

    if (fila == 1) {
        for (int col = 0; col < m; col++) {
            actual.push_back(col);
            // CAMBIO: energia[fila-1][col] en vez de energia[fila][col]
            // porque ahora fila 0 de datos = energia[0], fila 1 = energia[1], etc.
            FB(energia, fila + 1, mejor, actual, mejorEnergia, energiaActual + energia[fila - 1][col]);
            actual.pop_back();
        }
    } else {
        int ult_columna = actual.back();

        for (int col = ult_columna - 1; col <= ult_columna + 1; col++) {
            if (col >= 0 && col < m) {
                actual.push_back(col);
                // CAMBIO: energia[fila-1][col] en vez de energia[fila][col]
                FB(energia, fila + 1, mejor, actual, mejorEnergia, energiaActual + energia[fila - 1][col]);
                actual.pop_back();
            }
        }
    }
}


std::vector<int> encontrarSeamFuerzaBruta(const std::vector<std::vector<double>>& energia) {
    // CAMBIO: n se lee con .size() en vez de energia[0][0]
    int n = energia.size();

    double mejorEnergia = std::numeric_limits<double>::infinity();

    std::vector<int> mejor;
    std::vector<int> actual = {};
    FB(energia, 1, mejor, actual, mejorEnergia, 0);

    // CAMBIO: se elimina el bucle que sumaba +1 a cada columna,
    // porque main.cpp usa el seam en 0-indexed directamente
    return mejor;
}