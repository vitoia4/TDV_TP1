import math
import sys
sys.setrecursionlimit(10000)

def PD(energia, memo, fila, col):
    n = len(energia)
    m = len(energia[0])

    if col < 0 or col >= m:
        return math.inf

    if fila == n - 1:
        memo[fila][col] = energia[fila][col]
        return memo[fila][col]

    if memo[fila][col] != -1.0:
        return memo[fila][col]
    else:
        abajo     = PD(energia, memo, fila + 1, col)
        abajo_izq = PD(energia, memo, fila + 1, col - 1)
        abajo_der = PD(energia, memo, fila + 1, col + 1)

        mejor = min(abajo, abajo_der, abajo_izq)

        memo[fila][col] = energia[fila][col] + mejor
        return memo[fila][col]


def encontrar_seam_pd(energia):
    n = len(energia)
    m = len(energia[0])

    memo = [[-1.0] * m for _ in range(n)]

    for j in range(m):
        PD(energia, memo, 0, j)

    # Elegir columna inicial con menor energía en fila 0
    mejor_col = 0
    menor_energia = memo[0][0]

    for j in range(1, m):
        if memo[0][j] < menor_energia:
            mejor_col = j
            menor_energia = memo[0][j]

    rta = [mejor_col]

    for i in range(1, n):
        ult_col = rta[-1]

        izq = math.inf
        der = math.inf

        ab = memo[i][ult_col]

        if ult_col > 0:
            izq = memo[i][ult_col - 1]
        if ult_col < m - 1:
            der = memo[i][ult_col + 1]

        if izq <= ab and izq <= der:
            rta.append(ult_col - 1)
        elif der <= ab and der <= izq:
            rta.append(ult_col + 1)
        else:
            rta.append(ult_col)

    return rta