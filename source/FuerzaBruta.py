def encontrar_seam_backtracking(energia):
    mejor = []

    # inicializo mejor con la columna vertical 0
    for _ in range(len(energia)):
        mejor.append(0)

    actual = []
    return bt(energia, 0, mejor, actual)


def calcular_energia_seam(energia_matriz, seam):
    total = 0.0
    for i in range(len(seam)):
        total += energia_matriz[i][seam[i]]
    return total


def bt(energia, fila, mejor, actual):
    if fila == len(energia):
        if calcular_energia_seam(energia, actual) < calcular_energia_seam(energia, mejor):
            mejor[:] = actual
        return mejor

    if fila == 0:
        for col in range(len(energia[1])):
            actual.append(col)
            bt(energia, fila + 1, mejor, actual)
            actual.pop()

    else:
        # Poda: no seguir costura si energia actual > energia total mejor
        # Poda: recorrer solo casilleros válidos por definición de costura vertical
        if calcular_energia_seam(energia, actual) < calcular_energia_seam(energia, mejor):
            ult_columna = actual[-1]

            actual.append(ult_columna)
            bt(energia, fila + 1, mejor, actual)
            actual.pop()

            if ult_columna > 0:  # primer columna
                actual.append(ult_columna - 1)
                bt(energia, fila + 1, mejor, actual)
                actual.pop()

            if ult_columna < len(energia[1]) - 1:  # ultima columna
                actual.append(ult_columna + 1)
                bt(energia, fila + 1, mejor, actual)
                actual.pop()

    return mejor