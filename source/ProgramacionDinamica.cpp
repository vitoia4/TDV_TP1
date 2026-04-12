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


//----------------------------------------------------------------------------
double calcularEnergiaSeam(const std::vector<std::vector<double>>& energiaMatriz, const std::vector<int>& seam) {
    double total = 0.0;
    for (int i = 0; i < seam.size(); i++) {
        total += energiaMatriz[i+1][seam[i]-1];
    }
    return total;
}

// === FUNCION AUXILIAR ===
void testCaso(const std::vector<std::vector<double>>& energia,
              const std::string& nombre,
              const std::vector<int>& esperado) {

    std::cout << "==== " << nombre << " ====" << std::endl;

    std::vector<int> seam = encontrarSeamPD(energia);

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

    // 🧪 Caso 1: básico
    std::vector<std::vector<double>> caso1 = {
        {3, 3},
        {1, 2, 3},
        {4, 1, 6},
        {7, 8, 1}
    };
    testCaso(caso1, "Caso 1 (basico)", {1, 2, 3});

    // 🧪 Caso 2: todos iguales
    std::vector<std::vector<double>> caso2 = {
        {4, 4},
        {1, 1, 1, 1},
        {1, 1, 1, 1},
        {1, 1, 1, 1},
        {1, 1, 1, 1}
    };
    testCaso(caso2, "Caso 2 (todos iguales)", {});

    // 🧪 Caso 3: columna óptima clara
    std::vector<std::vector<double>> caso3 = {
        {4, 4},
        {1, 100, 100, 100},
        {1, 100, 100, 100},
        {1, 100, 100, 100},
        {1, 1,   1,   1}
    };
    testCaso(caso3, "Caso 3 (columna fija)", {1, 1, 1, 1});

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

    // 🧪 Caso 5: tu ejemplo conceptual
    std::vector<std::vector<double>> caso5 = {
        {4, 4},
        {2, 2, 2, 1},
        {0.5, 1, 3, 3},
        {1, 1, 2, 2},
        {1, 1, 1, 1}
    };
    testCaso(caso5, "Caso 5 (DP conceptual)", {1, 1, 1, 1});

    // 🧪 Caso 6: enunciado
    std::vector<std::vector<double>> caso6 = {
        {5, 6},
        {9.0, 3.0, 1.0, 2.0, 8.0, 7.0},
        {5.0, 2.0, 0.5, 4.0, 6.0, 3.0},
        {7.0, 1.0, 2.0, 0.8, 5.0, 4.0},
        {3.0, 4.0, 1.5, 1.0, 2.0, 6.0},
        {8.0, 2.0, 3.0, 0.5, 1.0, 5.0}
    };
    testCaso(caso6, "Caso 6 (enunciado)", {3, 3, 4, 4, 4});

    return 0;
}