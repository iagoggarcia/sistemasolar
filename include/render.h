#ifndef RENDER_H
#define RENDER_H

#include <glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "cuerpo.h"

GLFWwindow* inicializar();
void actualizarEstado(GLFWwindow* window, std::vector<CuerpoCeleste*>& cuerpos);
void renderizar(std::vector<CuerpoCeleste*>& cuerpos, GLuint modelLoc, GLuint colorLoc, GLuint viewLoc, GLuint projectionLoc, GLFWwindow* window);
void limpiar(std::vector<CuerpoCeleste*>& cuerpos);

#endif