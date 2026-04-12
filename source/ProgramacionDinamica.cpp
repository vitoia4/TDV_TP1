#include "ProgramacionDinamica.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <vector>

double PD(const std::vector<std::vector<double>>& energia, std::vector<std::vector<double>> & memo, int fila, int col){
    int n = energia[0][0];
    int m = energia[0][1];
    
    if (col<0 || col>=m){
        return std::numeric_limits<double>::infinity();     
    }
    
    if (fila == n-1){
        memo[fila][col] = energia[fila+1][col];
        return memo[fila][col];
    }

    if (memo[fila][col] != -1.0){  
        return memo[fila][col];
    }

    else{
        double abajo = PD(energia, memo, fila+1, col);
        double abajo_izq = PD(energia, memo, fila+1, col-1);
        double abajo_der = PD(energia, memo, fila+1, col+1);
        
        double mejor = std::min({abajo, abajo_der, abajo_izq});

        memo[fila][col] = energia[fila+1][col] + mejor;
        return memo[fila][col];
    }
}


std::vector<int> encontrarSeamPD(const std::vector<std::vector<double>>& energia) {
    int n = energia[0][0];
    int m = energia[0][1];

    std::vector<std::vector<double>> memo(n, std::vector<double>(m, -1.0));

    for(int j=0; j<m; j++){
        PD(energia, memo, 0, j);
    }

    //recuperar recorrido
    int mejorCol = 0;
    double menorEnergia = memo[0][0];

    for(int j=1; j<m; j++){
        if (memo[0][j] < menorEnergia){
            mejorCol = j;
            menorEnergia = memo[0][j];
        }
    }

    std::vector<int> rta;
    rta.push_back(mejorCol);

    for(int i=1; i<n; i++){
        int ult_col = rta.back();

        double izq = std::numeric_limits<double>::infinity();
        double der = std::numeric_limits<double>::infinity();

        double ab = memo[i][ult_col];

        if(ult_col>0){
            izq = memo[i][ult_col-1];
        }
        if(ult_col<m-1){
            der = memo[i][ult_col+1];
        }

        double minimo = std::min({izq,ab,der});

        if(izq <= ab && izq <= der){
            rta.push_back(ult_col-1);
        }
        else if (der <= ab && der <= izq){
            rta.push_back(ult_col+1);
        }
        else {
            rta.push_back(ult_col);
        }
    }
    
    //Para que la respuesta este indexado en 1
    for(int i=0; i<n; i++){
        rta[i]++;
    }

    return rta;
}