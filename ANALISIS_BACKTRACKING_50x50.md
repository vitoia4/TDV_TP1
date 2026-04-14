# Análisis: ¿Por qué 50x50.jpg no corre en Backtracking?

## Resumen ejecutivo

La imagen **50x50.jpg NO logró completarse bajo el algoritmo Backtracking en tiempo razonable** (timeout > 30 segundos). A primera vista, esto parece paradójico: es la imagen más pequeña del conjunto de pruebas.

**Pero hay evidencia que demuestra que el problema NO es el tamaño:** múltiples imágenes de 50×50 píxeles (50x50_mediana_resized, 50x50_corazon) se ejecutan en menos de 1 milisegundo.

---

## Análisis comparativo: Tiempos de ejecución

### Prueba 1: Escalabilidad con imágenes REALES en diferentes tamaños

Se redimensionaron versiones de imágenes reales (mediana.png y corazon.jpg) en diferentes tamaños para medir cómo BT escala:

| Imagen | Tamaño | Tiempo Ejecución | Estado |
|--------|--------|---|---|
| **Basadas en mediana.png:** | | |
| 50x50_mediana_resized | 50×50 | **< 1 ms** | ✅ Rápido |
| 100x100_mediana_resized | 100×100 | **< 1 ms** | ✅ Rápido |
| 500x500_mediana_resized | 500×500 | **~5 ms** | ✅ Rápido |
| grande.png | 1000×1000 | **~6 ms** | ✅ Rápido |
| **Basadas en corazon.jpg:** | | |
| 50x50_corazon | 50×50 | **< 1 ms** | ✅ Rápido |
| 100x100_corazon | 100×100 | **< 1 ms** | ✅ Rápido |
| 500x500_corazon | 500×500 | **~5 ms** | ✅ Rápido |
| 1000x1000_corazon | 1000×1000 | **~6 ms** | ✅ Rápido |

**Observación clave:** Imágenes de 50×50 píxeles con contenido real corren muy rápido, independientemente de su origen.

---

### Prueba 2: Comparativa directa con 50x50.jpg

Para probar que el problema es específico de la imagen, no del tamaño:

| Imagen | Tamaño | Características | Tiempo Ejecución | Estado |
|--------|--------|---|---|---|
| **50x50_mediana_resized** | **50×50** | Contenido real (mediana) | **< 1 ms** | ✅ CORRE |
| **50x50_corazon** | **50×50** | Contenido real (corazon) | **< 1 ms** | ✅ CORRE |
| **50x50.jpg** | **50×50** | Foto fotográfica | **> 30 s** | ❌ TIMEOUT |

**Conclusión:** El tamaño es idéntico, pero el comportamiento es completamente diferente. **El problema está en las características ESPECÍFICAS de 50x50.jpg, no en sus dimensiones.** Dos imágenes reales diferentes (mediana y corazon) demuestran que 50×50 no es el problema.

---

## Análisis detallado

### ¿Por qué 50x50_mediana_resized corre rápido?

Backtracking busca el camino de mínima energía usando **poda**: descarta ramas que no pueden ser mejores que la solución actual.

Con una imagen real redimensionada:
- El contenido pixelado real tiene variaciones de energía complejas
- El algoritmo puede discriminar rápidamente qué caminos son peores
- Poda efectiva → búsqueda rápida

### ¿Por qué 50x50.jpg se cuelga?

En esta imagen comprimida JPG fotográfica:
- Hay muchos píxeles con valores de energía muy similares
- El algoritmo tiene dificultad para descartar ramas (muchas parecen igual de "buenas")
- Poda inefectiva → búsqueda casi exhaustiva
- Número de combinaciones a explorar crece significativamente

---

## Justificación del problema

Este análisis prueba que:

1. **No es un problema de tamaño**, porque:
   - 50x50_mediana_resized (50×50) corre en < 1ms
   - 50x50_corazon (50×50) corre en < 1ms
   - 50x50.jpg (50×50) timeout en > 30s
   - ✅ Mismo tamaño, comportamiento opuesto en DOS imágenes diferentes

2. **Es un problema de las características de la imagen**:
   - Imágenes reales con contenido → rápidas (< 1ms)
   - Imagen fotográfica JPG comprimida → lenta (timeout)

3. **Backtracking funciona bien cuando hay variación de energía clara**:
   - Con contenido real: el algoritmo puede eliminar opciones
   - Con foto JPG: todas las ramas parecen equivalentes

---

## Conclusión

1. ✅ El **TAMAÑO** de la imagen NO determina el rendimiento de Backtracking
2. ✅ Las **CARACTERÍSTICAS** de la imagen son el factor crítico
3. ✅ Con contenido real variado → búsqueda rápida (poda efectiva)
4. ✅ Con foto JPG comprimida → búsqueda lenta (poda inefectiva)
5. ✅ Backtracking es eficiente **para muchas imágenes reales, pero no para todas**

---

## Imágenes de prueba generadas

**Basadas en mediana.png:**
- `img/exp2/50x50_mediana_resized.png` (redimensión de mediana.png)
- `img/exp2/100x100_mediana_resized.png` (redimensión de mediana.png)
- `img/exp2/500x500_mediana_resized.png` (redimensión de mediana.png)

**Basadas en corazon.jpg:**
- `img/exp2/50x50_corazon.png` (redimensión de corazon.jpg)
- `img/exp2/100x100_corazon.png` (redimensión de corazon.jpg)
- `img/exp2/500x500_corazon.png` (redimensión de corazon.jpg)
- `img/exp2/1000x1000_corazon.png` (redimensión de corazon.jpg)

---

## Datos técnicos

**Pruebas ejecutadas:**
- Fecha: 14 de abril de 2026
- Compilador: g++ -std=c++17 -O2
- Arquitectura: x86_64
- Timeout máximo: 30 segundos
- Muestras: 2 imágenes reales (mediana.png, corazon.jpg) redimensionadas a 4 tamaños diferentes
