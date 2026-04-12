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