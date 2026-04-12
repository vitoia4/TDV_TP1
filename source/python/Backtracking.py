import math


def BT(energia, fila, mejor, actual, mejor_energia, energia_actual):
    n = len(energia)
    m = len(energia[0])

    if fila == n + 1:
        if energia_actual <= mejor_energia[0]:
            mejor[:] = actual[:]
            mejor_energia[0] = energia_actual
        return

    if fila == 1:
        for col in range(m):
            actual.append(col)
            BT(energia, fila + 1, mejor, actual, mejor_energia, energia_actual + energia[fila - 1][col])
            actual.pop()
    else:
        # Poda: no seguir costura si energia actual >= energia total mejor
        if energia_actual < mejor_energia[0]:
            ult_columna = actual[-1]

            for col in range(ult_columna - 1, ult_columna + 2):
                if 0 <= col < m:
                    actual.append(col)
                    BT(energia, fila + 1, mejor, actual, mejor_energia, energia_actual + energia[fila - 1][col])
                    actual.pop()


def encontrar_seam_backtracking(energia):
    n = len(energia)

    mejor_energia = [math.inf]
    mejor = []
    actual = []

    BT(energia, 1, mejor, actual, mejor_energia, 0)

    return mejor