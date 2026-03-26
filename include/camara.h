#ifndef CAMARA_H
#define CAMARA_H

#include <glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "planetas.h"
#include "satelites.h"


extern Planeta* planetaObjetivo;
extern Satelite* sateliteObjetivo;
extern float distanciaCamara;
extern glm::vec3 direccionCamara;
extern bool vistaDesdeLuna;
extern bool vistaDesdeTierra;
extern glm::vec3 posicionCamaraGeneral;
extern glm::vec3 direccionCamaraGeneral;
extern glm::vec3 upCamaraGeneral;

// Función que analiza qué teclas se tocan para ajustar la posición de la cámara y la vista
void processInput(GLFWwindow* window, std::vector<Planeta*>& planetas, std::vector<Satelite*>& satelites);

glm::mat4 obtenerVista(std::vector<Planeta*>& planetas, std::vector<Satelite*>& satelites);

#endif