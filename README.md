# 🌌 Sistema Solar — Entrega 1 (COGA)

Pequeña simulación de un sistema solar desarrollada con **OpenGL** como parte de la **Entrega 1 de la asignatura COGA**.

---

# ✅ Funcionalidades implementadas

- ✔️ Renderizado de planetas utilizando una **esfera base reutilizada**.
- ✔️ Uso de **estructuras (`struct Planeta`)** para almacenar los datos de cada planeta.
- ✔️ Posicionamiento de los planetas según su **distancia al Sol en el eje X**.
- ✔️ Tamaños específicos para cada planeta mediante **transformaciones de escala**.
- ✔️ Colores individuales para cada planeta enviados al **fragment shader**.
- ✔️ Modularización del código separando:
  - `main.cpp` → control del programa
  - `planetas.cpp / planetas.h` → lógica de los planetas
  - `satelites.cpp / satelites.h` → lógica de los satélites
- ✔️ Renderizado de todos los planetas mediante una función dedicada (`dibujarPlanetas`).
- ✔️ Implementación del **sistema de cámara** para visualizar correctamente el sistema solar.
- ✔️ Implementación de **movimiento orbital de los planetas** usando:
  - ángulos de traslación
  - velocidades de rotación y traslación.
- ✔️ Añadir **satélites**:
  - 🌙 Luna
  - 🛰️ Estación Espacial Internacional (ISS).
- Representación visual de **órbitas**.
---

# 🚧 Funcionalidades pendientes
- 🔲 Representar visualmente **anillos planetarios**.