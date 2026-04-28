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

// utilizamos dos shaders distintos: uno para la iluminación y texturas, y otro para dibujar las esferas
// y las órbitas
GLuint shaderIluminacionProgram;
GLuint shaderSimpleProgram;

GLuint VAO_esfera, VBO_esfera;

void reescalarVentana(GLFWwindow*, int w, int h) {
    glViewport(0, 0, w, h);
}

int main() {
    GLFWwindow* window = inicializar();
    if (!window) return -1;

    // asignamos los shaders para usarlos donde haga falta en las funciones
    shaderIluminacionProgram = setShaders("shaders/iluminacion.vert", "shaders/iluminacion.frag");
    shaderSimpleProgram      = setShaders("shaders/simple.vert", "shaders/simple.frag");

    crearEsfera(VAO_esfera, VBO_esfera);

    // generamos el vector con los cuerpos (planetas, luna e ISS) con todos sus datos en un struct
    std::vector<CuerpoCeleste*> cuerpos = inicializarCuerpos(VAO_esfera);

    while (!glfwWindowShouldClose(window)) { // mientras no cerremos la ventana (con 'esc')
        actualizarEstado(window, cuerpos);
        renderizar(cuerpos, window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    limpiar(cuerpos); // liberamos los struct de todos los planetas cuando salgamos del programa
    return 0;
}