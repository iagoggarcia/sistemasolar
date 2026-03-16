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
- ✔️ Renderizado de todos los planetas mediante una función dedicada (`dibujarPlanetas`).
- ✔️ Implementar **sistema de cámara** (`view` y `projection`) para visualizar correctamente el sistema solar.
- ✔️ Ajustar **tamaños y distancias** una vez se implemente la cámara.
- ✔️ Implementar **movimiento orbital de los planetas** usando:
  - ángulos de traslación
  - velocidades de rotación y traslación.

---

# 🚧 Funcionalidades pendientes

- 🔲 Añadir **satélites**:
  - 🌙 Luna
  - 🛰️ Estación Espacial Internacional (ISS).
- 🔲 Representar visualmente **órbitas y anillos planetarios**.
---
# 📂 Estructura actual del proyecto
