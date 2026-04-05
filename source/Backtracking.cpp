#include "Backtracking.h"
#include <cstddef>

std::vector<int> encontrarSeamBacktracking(const std::vector<std::vector<double>>& energia) {
    std::vector<int> mejor =  {};
    std::vector<int> actual =  {};
    return BT(energia, 0, mejor, actual);
}

double calcularEnergiaSeam(const std::vector<std::vector<double>>& energiaMatriz, const std::vector<int>& seam) {
    double total = 0.0;
    for (size_t i = 0; i < seam.size(); i++) {
        total += energiaMatriz[i][seam[i]];
    }
    return total;
}

std::vector<int> BT(const std::vector<std::vector<double>>& energia, int fila, std::vector<int>&mejor, std::vector<int> actual) {
    if(fila == energia.size()){
        if(calcularEnergiaSeam(energia, actual) < calcularEnergiaSeam(energia, mejor)){
            mejor = actual;
            
        }
        return mejor;

    }

    if(fila==0){
        for (int col=0; col < energia[1].size(); col++){
            actual.push_back(col);
            BT(energia, fila+1, mejor, actual);
            actual.pop_back();
            }
        }

    else{
        int ult_columna = actual[fila-1];

        actual.push_back(ult_columna);
        BT(energia, fila+1, mejor, actual);
        actual.pop_back();

        if(ult_columna>0){ // primer columna
            actual.push_back(ult_columna-1);
            BT(energia, fila+1, mejor, actual);
            actual.pop_back();
        }
        if(ult_columna<energia[1].size()){ // ultima columna
            actual.push_back(ult_columna+1);
            BT(energia, fila+1, mejor, actual);
            actual.pop_back();
        }
        
    }
    return mejor;

}