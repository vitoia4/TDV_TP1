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