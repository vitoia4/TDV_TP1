"""
main.py — equivalente Python de main.cpp
Uso:
  Modo numérico: python main.py --numerico <archivo> --algoritmo <fb|bt|pd>
  Modo imagen:   python main.py --imagen <archivo>   --algoritmo <fb|bt|pd> --iteraciones <N>

Las rutas de entrada pueden darse relativas a la RAÍZ DEL PROYECTO o como
rutas absolutas. El script las resuelve correctamente sin importar desde
qué directorio de trabajo se lo invoque.

La medición de tiempo incluye SOLO la ejecución del algoritmo,
NO la carga de imagen ni el cálculo de la matriz de energía.
"""

import argparse
import sys
import time
from pathlib import Path

from Imagen import Imagen
from FuerzaBruta import encontrar_seam_fuerza_bruta
from Backtracking import encontrar_seam_backtracking
from ProgramacionDinamica import encontrar_seam_pd


# ── Raíz del proyecto ─────────────────────────────────────────────────────────
# __file__ es source/python/main.py  →  .parent = source/python/
#                                        .parent.parent = source/
#                                        .parent.parent.parent = proyecto/  ← raíz
PROJECT_ROOT = Path(__file__).resolve().parent.parent.parent


def resolver_ruta(ruta_usuario: str) -> Path:
    """
    Convierte la ruta dada por el usuario en una Path absoluta resuelta.

    Estrategia:
      1. Si ya es absoluta, se usa tal cual.
      2. Si es relativa, se intenta primero relativa al directorio de trabajo
         actual (cwd). Si no existe allí, se prueba relativa a PROJECT_ROOT.
         Esto permite tanto:
           python source/python/main.py --imagen img/ejemplo.png   (desde raíz)
           python main.py --imagen img/ejemplo.png                  (desde source/python/)
    """
    p = Path(ruta_usuario)
    if p.is_absolute():
        return p
    # Relativa al cwd (caso más común cuando se ejecuta desde la raíz)
    desde_cwd = Path.cwd() / p
    if desde_cwd.exists():
        return desde_cwd.resolve()
    # Fallback: relativa a la raíz del proyecto
    return (PROJECT_ROOT / p).resolve()


def ruta_salida(subdir: str, nombre: str) -> Path:
    """Devuelve PROJECT_ROOT/output/<subdir>/<nombre> y crea el directorio."""
    destino = PROJECT_ROOT / "output" / subdir / nombre
    destino.parent.mkdir(parents=True, exist_ok=True)
    return destino


# ── Utilidades ────────────────────────────────────────────────────────────────

def leer_matriz_energia(ruta: str) -> list[list[float]]:
    """
    Formato esperado:
      filas columnas
      e00 e01 ...
      ...
    """
    with open(resolver_ruta(ruta)) as f:
        filas, columnas = map(int, f.readline().split())
        energia = []
        for _ in range(filas):
            fila = list(map(float, f.readline().split()))
            energia.append(fila)
    return energia


def ejecutar_algoritmo(energia: list[list[float]], algoritmo: str) -> list[int]:
    if algoritmo == "fb":
        return encontrar_seam_fuerza_bruta(energia)
    if algoritmo == "bt":
        return encontrar_seam_backtracking(energia)
    if algoritmo == "pd":
        return encontrar_seam_pd(energia)
    raise ValueError(f"Algoritmo desconocido: '{algoritmo}'. Usar fb, bt o pd.")


def imprimir_matriz(energia: list[list[float]]) -> None:
    for fila in energia:
        print("\t".join(f"{v:.4f}" for v in fila))


def imprimir_seam(seam: list[int], energia: list[list[float]]) -> None:
    total = sum(energia[f][seam[f]] for f in range(len(seam)))
    coords = " ".join(f"({f},{seam[f]})" for f in range(len(seam)))
    print(f"Seam encontrado: {coords}")
    print(f"Energía total: {total}")


# ── Modos de ejecución ────────────────────────────────────────────────────────

def modo_numerico(ruta_entrada: str, algoritmo: str) -> None:
    energia = leer_matriz_energia(ruta_entrada)

    print("Matriz de energía:")
    imprimir_matriz(energia)
    print()

    # Medir SOLO el algoritmo
    inicio = time.perf_counter()
    seam = ejecutar_algoritmo(energia, algoritmo)
    fin = time.perf_counter()

    elapsed = fin - inicio
    print(f"Tiempo (segundos): {elapsed:.9f}")
    imprimir_seam(seam, energia)

    destino = ruta_salida("numericos", f"seam_{algoritmo}.txt")
    with open(destino, "w") as f:
        for fila_idx, col in enumerate(seam):
            f.write(f"fila {fila_idx} -> columna {col}\n")
    print(f"Resultado guardado en {destino}")


def modo_imagen(ruta_imagen: str, algoritmo: str, iteraciones: int) -> None:
    img = Imagen(resolver_ruta(ruta_imagen))
    print(f"Imagen cargada: {img.ancho()}x{img.alto()} px")

    tiempo_total = 0.0

    for i in range(iteraciones):
        # Cálculo de energía NO incluido en la medición
        energia = img.obtener_matriz_energia()

        inicio = time.perf_counter()
        seam = ejecutar_algoritmo(energia, algoritmo)
        fin = time.perf_counter()

        tiempo_total += fin - inicio

        img.eliminar_seam(seam)

        if (i + 1) % 10 == 0 or i == iteraciones - 1:
            print(f"Iteración {i + 1}/{iteraciones} - Ancho actual: {img.ancho()} px")

    print(f"Tiempo total algoritmo (segundos): {tiempo_total:.9f}")
    print(f"Tiempo promedio por iteración: {tiempo_total / iteraciones:.9f}")

    destino = ruta_salida("imagenes", f"resultado_{algoritmo}.png")
    img.guardar(destino)
    print(f"Imagen guardada en {destino}")


# ── Entry point ───────────────────────────────────────────────────────────────

def main() -> None:
    parser = argparse.ArgumentParser(
        description="Seam Carving — experimentación Python",
        formatter_class=argparse.RawTextHelpFormatter,
    )

    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("--numerico", metavar="ARCHIVO",
                       help="Modo numérico: lee una matriz de energía desde un .txt")
    group.add_argument("--imagen", metavar="ARCHIVO",
                       help="Modo imagen: carga una imagen y aplica seam carving")

    parser.add_argument("--algoritmo", choices=["fb", "bt", "pd"], default="pd",
                        help="Algoritmo a usar: fb (fuerza bruta), bt (backtracking), pd (prog. dinámica)")
    parser.add_argument("--iteraciones", type=int, default=1,
                        help="(Modo imagen) Número de costuras a eliminar")

    args = parser.parse_args()

    try:
        if args.numerico:
            modo_numerico(args.numerico, args.algoritmo)
        else:
            modo_imagen(args.imagen, args.algoritmo, args.iteraciones)
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()