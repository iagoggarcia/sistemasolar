#ifndef INPUT_H
#define INPUT_H

#include <glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "cuerpo.h"

void cerrarVentanaSiEsc(GLFWwindow* window);

void procesarTeclaOrbitas(GLFWwindow* window);
void procesarTeclasVelocidad(GLFWwindow* window);
void procesarMovimientoCamaraGeneral(GLFWwindow* window, float deltaTime);
// Función encargada de llamar a las funciones de camara.cpp/h para que enfoquen a un determinado planeta según la tecla pulsada
void procesarTeclasEnfoque(GLFWwindow* window, std::vector<CuerpoCeleste*>& cuerpos);

// Función principal
void processInput(GLFWwindow* window, std::vector<CuerpoCeleste*>& cuerpos, float deltaTime);

#endif