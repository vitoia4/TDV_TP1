#include "FuerzaBruta.h"
#include <cstddef>

double calcularEnergiaSeam(const std::vector<std::vector<double>>& energiaMatriz, const std::vector<int>& seam) {
    double total = 0.0;
    for (size_t i = 0; i < seam.size(); i++) {
        total += energiaMatriz[i][seam[i]];
    }
    return total;
}

std::vector<int> FB(const std::vector<std::vector<double>>& energia, int fila, std::vector<int> mejor, std::vector<int> actual) {
    if(fila == energia.size()){
        if(calcularEnergiaSeam(energia, actual) < calcularEnergiaSeam(energia, mejor)){
            mejor = actual;
            return mejor;
        }

    }
    else{
        for (int col=0; col < energia[1].size()-1; col++){
            actual.push_back(col);
            FB(energia, fila+1, mejor, actual);
            actual.pop_back();

            if(col>0){ // primer columna
                actual.push_back(col-1);
                FB(energia, fila+1, mejor, actual);
                actual.pop_back();
            }
            if(col<energia[1].size()){ // ultima columna
                actual.push_back(col+1);
                FB(energia, fila+1, mejor, actual);
                actual.pop_back();
            }
        }
    }

}




std::vector<int> encontrarSeamFuerzaBruta(const std::vector<std::vector<double>>& energia) {
    std::vector<int> mejor =  {};
    std::vector<int> actual =  {};
    return FB(energia, 0, mejor, actual);
}
