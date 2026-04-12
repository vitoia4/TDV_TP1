"""
Imagen.py — equivalente Python de Imagen.cpp
Usa Pillow para carga/guardado y NumPy para el cálculo de energía.

Las rutas de entrada/salida pueden ser str o pathlib.Path; ambas son aceptadas.
"""

import numpy as np
from pathlib import Path
from PIL import Image
import os


class Imagen:
    def __init__(self, ruta: str | Path | None = None):
        self._pixeles: np.ndarray | None = None  # shape (alto, ancho, 3)
        self._alto = 0
        self._ancho = 0
        if ruta is not None:
            self.cargar(ruta)

    # ── I/O ──────────────────────────────────────────────────────────────────

    def cargar(self, ruta: str | Path) -> None:
        ruta = Path(ruta).resolve()

        if not ruta.exists():
            raise FileNotFoundError(f"No se encontró la imagen: {ruta}")

        with Image.open(ruta) as img:
            img = img.convert("RGB")
            self._pixeles = np.array(img, dtype=np.uint8)

        self._alto, self._ancho = self._pixeles.shape[:2]

    def guardar(self, ruta: str | Path) -> None:
        if self.esta_vacia():
            raise RuntimeError("La imagen está vacía.")

        ruta = Path(ruta).resolve()
        ruta.parent.mkdir(parents=True, exist_ok=True)

        Image.fromarray(self._pixeles).save(ruta)

    # ── Propiedades ───────────────────────────────────────────────────────────

    def ancho(self) -> int:
        return self._ancho

    def alto(self) -> int:
        return self._alto

    def esta_vacia(self) -> bool:
        return self._ancho == 0 or self._alto == 0

    # ── Energía ───────────────────────────────────────────────────────────────

    def obtener_matriz_energia(self) -> list[list[float]]:
        if self._pixeles is None:
            raise RuntimeError("La imagen está vacía.")

        p = self._pixeles.astype(np.float64)

        # Gradiente horizontal
        p_right = p[:, np.minimum(np.arange(self._ancho) + 1, self._ancho - 1), :]
        p_left  = p[:, np.maximum(np.arange(self._ancho) - 1, 0), :]
        dx = p_right - p_left

        # Gradiente vertical
        rows = np.arange(self._alto)
        p_down = p[np.minimum(rows + 1, self._alto - 1), :, :]
        p_up   = p[np.maximum(rows - 1, 0), :, :]
        dy = p_down - p_up

        energia_arr = np.sqrt((dx**2 + dy**2).sum(axis=2))

        return energia_arr.tolist()

    # ── Eliminación de seam ───────────────────────────────────────────────────

    def eliminar_seam(self, seam: list[int]) -> None:
        if self._pixeles is None or len(seam) != self._alto:
            raise RuntimeError("El seam debe tener exactamente una entrada por fila.")

        if self._ancho <= 1:
            raise RuntimeError("No se puede eliminar seam: ancho <= 1.")

        nuevo = np.zeros((self._alto, self._ancho - 1, 3), dtype=np.uint8)

        for f in range(self._alto):
            col = seam[f]

            if col < 0 or col >= self._ancho:
                raise ValueError(f"Índice inválido en seam: fila {f}, columna {col}")

            nuevo[f, :col, :] = self._pixeles[f, :col, :]
            nuevo[f, col:, :] = self._pixeles[f, col + 1:, :]

        self._pixeles = nuevo
        self._ancho -= 1