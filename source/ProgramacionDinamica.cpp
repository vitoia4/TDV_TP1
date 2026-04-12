#include "ProgramacionDinamica.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <vector>

double PD(const std::vector<std::vector<double>>& energia, std::vector<std::vector<double>>& memo, int fila, int col) {
    // CAMBIO: n y m se leen con .size() porque la matriz ya no tiene fila de dimensiones
    int n = energia.size();
    int m = energia[0].size();

    if (col < 0 || col >= m) {
        return std::numeric_limits<double>::infinity();
    }

    if (fila == n - 1) {
        // CAMBIO: energia[fila][col] en vez de energia[fila+1][col]
        memo[fila][col] = energia[fila][col];
        return memo[fila][col];
    }

    if (memo[fila][col] != -1.0) {
        return memo[fila][col];
    } else {
        double abajo     = PD(energia, memo, fila + 1, col);
        double abajo_izq = PD(energia, memo, fila + 1, col - 1);
        double abajo_der = PD(energia, memo, fila + 1, col + 1);

        double mejor = std::min({abajo, abajo_der, abajo_izq});

        // CAMBIO: energia[fila][col] en vez de energia[fila+1][col]
        memo[fila][col] = energia[fila][col] + mejor;
        return memo[fila][col];
    }
}


std::vector<int> encontrarSeamPD(const std::vector<std::vector<double>>& energia) {
    // CAMBIO: n y m se leen con .size() en vez de energia[0][0] y energia[0][1]
    int n = energia.size();
    int m = energia[0].size();

    std::vector<std::vector<double>> memo(n, std::vector<double>(m, -1.0));

    for (int j = 0; j < m; j++) {
        PD(energia, memo, 0, j);
    }

    // recuperar recorrido: elegir columna inicial con menor energía en fila 0
    int mejorCol = 0;
    double menorEnergia = memo[0][0];

    for (int j = 1; j < m; j++) {
        if (memo[0][j] < menorEnergia) {
            mejorCol = j;
            menorEnergia = memo[0][j];
        }
    }

    std::vector<int> rta;
    rta.push_back(mejorCol);

    for (int i = 1; i < n; i++) {
        int ult_col = rta.back();

        double izq = std::numeric_limits<double>::infinity();
        double der = std::numeric_limits<double>::infinity();

        double ab = memo[i][ult_col];

        if (ult_col > 0)   izq = memo[i][ult_col - 1];
        if (ult_col < m-1) der = memo[i][ult_col + 1];

        if (izq <= ab && izq <= der) {
            rta.push_back(ult_col - 1);
        } else if (der <= ab && der <= izq) {
            rta.push_back(ult_col + 1);
        } else {
            rta.push_back(ult_col);
        }
    }

    // CAMBIO: se elimina el bucle que sumaba +1 a cada columna,
    // porque main.cpp usa el seam en 0-indexed directamente
    return rta;
}