#include "ProgramacionDinamica.h"

double calcularEnergiaSeam(const std::vector<std::vector<double>>& energiaMatriz, const std::vector<int>& seam) {
    double total = 0.0;
    for (int i = 0; i < seam.size(); i++) {
        total += energiaMatriz[i][seam[i]];
    }
    return total;
}

std::vector<int> PD(const std::vector<std::vector<double>>& energia, std::vector<std::vector<std::vector<int>>>& memo, int i, int j) {
    if(i == energia.size()){
        return {j};
    }
    if(!memo[i][j].empty()){
        return memo[i][j];
    }


    if(i==0){
        for (int col=0; col < energia[1].size(); col++){
            
            memo[i][col] = BT(energia, i+1);
            
            }
        
        }

    else{
        // Ṕoda: no seguir costura si energia actual > energia total mejor
        // Poda: recorrer solo casilleros válidos por definición de costura vertical
        if(calcularEnergiaSeam(energia, actual) < calcularEnergiaSeam(energia, mejor)){
            int ult_columna = actual.back();

            actual.push_back(ult_columna);
            BT(energia, fila+1, mejor, actual);
            actual.pop_back();

            if(ult_columna>0){ // primer columna
                actual.push_back(ult_columna-1);
                BT(energia, fila+1, mejor, actual);
                actual.pop_back();
            }
            if(ult_columna<energia[1].size()-1){ // ultima columna
                actual.push_back(ult_columna+1);
                BT(energia, fila+1, mejor, actual);
                actual.pop_back();
            }
        }
        
    }


}

std::vector<int> encontrarSeamPD(const std::vector<std::vector<double>>& energia) {
    std::vector<int> mejor =  {};
    std::vector<std::vector<std::vector<int>>> memo = {};

    // inicializo memo con vectores vacíos
    for (int fila=0; fila < energia.size(); fila++){
        memo.push_back({}); 
        for (int col=0; fila < energia.size(); col++){ 
            memo[fila].push_back({});
        }
    }

    // inicializo mejor con la columna vertical 0
    for (int col=0; col < energia.size(); col++){ 
        mejor.push_back(0);
    }

    std::vector<int> actual =  {};

    std::vector<int> res = PD(energia, memo, 0, 0);

    // rango de columnas: 1 a n
    for (int i=0; i < res.size(); i++){
        res[i] += 1;
    }

    return res;
    
}
