// main.cpp
#include <glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "lecturaShader_0_9.h"
#include "esfera.h"
#include "cuerpo.h"
#include "camara.h"
#include "input.h"
#include "render.h"

const unsigned int SCR_WIDTH  = 1000;
const unsigned int SCR_HEIGHT = 1000;

float deltaTime   = 0.0f;
float ultimoFrame = 0.0f;

extern GLuint setShaders(const char* nVertx, const char* nFrag);
GLuint shaderProgram;
GLuint VAO_esfera, VBO_esfera;

void reescalarVentana(GLFWwindow*, int w, int h) {
    glViewport(0, 0, w, h);
}

int main() {
    GLFWwindow* window = inicializar();
    if (!window) return -1;

    shaderProgram = setShaders("shaders/shader.vert", "shaders/shader.frag");

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint colorLoc = glGetUniformLocation(shaderProgram, "color");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    crearEsfera(VAO_esfera, VBO_esfera);

    std::vector<CuerpoCeleste*> cuerpos = inicializarCuerpos(VAO_esfera);

    while (!glfwWindowShouldClose(window)) {
        actualizarEstado(window, cuerpos);
        renderizar(cuerpos, modelLoc, colorLoc, viewLoc, projectionLoc, window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    limpiar(cuerpos);
    return 0;
}