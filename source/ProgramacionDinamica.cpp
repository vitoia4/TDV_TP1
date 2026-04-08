#include "ProgramacionDinamica.h"
#include <limits>

double calcularEnergiaSeam(const std::vector<std::vector<double>>& energiaMatriz, const std::vector<int>& seam) {
    if (seam.empty()) {
        return std::numeric_limits<double>::infinity();
    }

    double total = 0.0;
    for (int i = 0; i < seam.size(); i++) {
        total += energiaMatriz[i + 1][seam[i]];
    }
    return total;
}

std::vector<int> PD(const std::vector<std::vector<double>>& energia, std::vector<std::vector<std::vector<int>>>& memo, int i, int j, std::vector<int> mejor, std::vector<int> actual) {
    // ultima fila
    if(i == energia[0][0]-1){
        return {j};
    }

    // busco estado en el
    if(!memo[i][j].empty()){
        return memo[i][j];
    }

    std::vector<int> mejor_local = mejor;

    if(i==0){
        for (int col = 0; col < energia[0][1]; col++) {
            actual.push_back(col);
            std::vector<int> candidato = PD(energia, memo, i + 1, col, mejor_local, actual);

            if (calcularEnergiaSeam(energia, candidato) < calcularEnergiaSeam(energia, mejor_local)) {
                mejor_local = candidato;
            }
            
            actual.pop_back();
        }                 
    }
    else {
            // Ṕoda: no seguir costura si energia actual > energia total mejor
            // Factibilidad: recorrer solo casilleros válidos por definición de costura vertical
            if (calcularEnergiaSeam(energia, actual) < calcularEnergiaSeam(energia, mejor_local)) {

                int ult_columna = actual.back();

                // desplazamientos: abajo (0), izquierda (-1), derecha (+1)
                for (int mov = -1; mov <= 1; mov++) {

                    int nueva_col = ult_columna + mov;

                    // validar límites
                    if (nueva_col < 0 || nueva_col >= energia[0][1]) continue;

                    actual.push_back(nueva_col);

                    std::vector<int> candidato = PD(energia, memo, i + 1, nueva_col, mejor_local, actual);

                    if (calcularEnergiaSeam(energia, candidato) < calcularEnergiaSeam(energia, mejor_local)) {
                        mejor_local = candidato;
                    }

                    actual.pop_back();
                }
            }

        
    }

    memo[i+1][j] = std::vector<int>(mejor_local.begin() + i, mejor_local.end());
    return mejor_local;
}

/*
PD(i,j):
    si estoy en última fila:
        return {j}

    si memo[i][j] existe:
        return memo[i][j]

    probar hijos válidos:
        (i+1, j)
        (i+1, j-1)
        (i+1, j+1)

    elegir el mejor hijo

    resultado = [j] + mejor_hijo

    memo[i][j] = resultado

    return resultado
*/

std::vector<int> PDD(const std::vector<std::vector<double>>& energia, std::vector<std::vector<std::vector<int>>>& memo, int i, int j) {
    // última fila
    if(i == energia[0][0]-1){ 
        return {j};
    }
    
    // chequeo memo
 
    // busco estado en el
    if(!memo[i][j].empty()){
        return memo[i][j];
    }   

    std::vector<int> mejor_hijo =  {};

    // desplazamientos: abajo (0), izquierda (-1), derecha (+1)
    for (int mov = -1; mov <= 1; mov++) {

        int nueva_col = j + mov;
        // validar límites
        if (nueva_col < 0 || nueva_col >= energia[0][1]) continue;
        std::vector<int> hijo = PDD(energia, memo, i + 1, j + mov);

        if (calcularEnergiaSeam(energia, hijo) < calcularEnergiaSeam(energia, mejor_hijo)) {
            mejor_hijo = hijo;
        }
    }

    std::vector<int> res = mejor_hijo;
    res.insert(res.begin(), j); // agrego columna actual
    memo[i][j] = res;
    return res;
    
}

/*std::vector<int> encontrarSeamPD(const std::vector<std::vector<double>>& energia) {
    std::vector<int> mejor =  {};
    std::vector<int> actual =  {};
    std::vector<std::vector<std::vector<int>>> memo = {};

    int n = energia[0][0];
    int m = energia[0][1];

    // inicializo memo con vectores vacíos
    std::vector<std::vector<std::vector<int>>> memo(n, std::vector<std::vector<int>>(m));

    // inicializo mejor con la priemera columna de energía
    for (int col=0; col < n; col++){ 
        mejor.push_back(0);
    }

    return PD(energia, memo, 0, 0, mejor, actual);
    
}
*/

std::vector<int> encontrarSeamPD(const std::vector<std::vector<double>>& energia) {
    std::vector<int> mejor =  {};
    std::vector<std::vector<std::vector<int>>> memo = {};

    int n = energia[0][0];
    int m = energia[0][1];

    // inicializo memo con vectores vacíos
    std::vector<std::vector<std::vector<int>>> memo(n, std::vector<std::vector<int>>(m));

    for (int j = 0; j < m; j++) {
        std::vector<int> col_actual = PDD(energia, memo, 0, j);
        if(calcularEnergiaSeam(energia, col_actual) < calcularEnergiaSeam(energia, mejor)){
            mejor = col_actual;
        }
    }
        // rango de columnas: 1 a n
    for (int i=0; i < mejor.size(); i++){
        mejor[i] += 1;
    }
    
    return mejor;
    
}

