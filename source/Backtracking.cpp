#include "Backtracking.h"
#include <cstddef>
#include <iostream>
#include <string>

double calcularEnergiaSeam(const std::vector<std::vector<double>>& energiaMatriz, const std::vector<int>& seam) {
    double total = 0.0;
    for (int i = 0; i < seam.size(); i++) {
        total += energiaMatriz[i+1][seam[i]];
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
    return mejor;
}

std::vector<int> encontrarSeamBacktracking(const std::vector<std::vector<double>>& energia) {
    std::vector<int> mejor =  {};
    // inicializo mejor con la columna vertical 0
    for (int col=0; col < energia.size(); col++){ 
        mejor.push_back(0);
    }
    std::vector<int> actual =  {};
    
    std::vector<int> res = BT(energia, 0, mejor, actual);

    // rango de columnas: 1 a n
    for (int i=0; i < res.size(); i++){
        res[i] += 1;
    }
    return res;
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