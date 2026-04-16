// render.cpp
#include <glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "cuerpo.h"
#include "camara.h"
#include "input.h"

extern float deltaTime;
extern float ultimoFrame;

GLFWwindow* inicializar() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1000, 1000, "Sistema Solar", NULL, NULL);
    if (!window) {
        std::cout << "Error creando ventana" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int w, int h) {
        glViewport(0, 0, w, h);
    });

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Error inicializando GLAD" << std::endl;
        return nullptr;
    }

    glClearDepth(1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    return window;
}

void actualizarEstado(GLFWwindow* window, std::vector<CuerpoCeleste*>& cuerpos) {
    float tiempoActual = glfwGetTime();
    deltaTime   = tiempoActual - ultimoFrame;
    ultimoFrame = tiempoActual;

    processInput(window, cuerpos, deltaTime);
    actualizarMovimiento(cuerpos, deltaTime, factorVelocidad);
}

void renderizar(std::vector<CuerpoCeleste*>& cuerpos, GLuint modelLoc, GLuint colorLoc, GLuint viewLoc, GLuint projectionLoc, GLFWwindow* window) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    extern GLuint shaderProgram;
    glUseProgram(shaderProgram);

    glm::mat4 view = obtenerVista(cuerpos);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glm::mat4 projection = glm::perspective(
        glm::radians(60.0f), (float)w / (float)h, 0.1f, 100.0f
    );
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    if (mostrarOrbitas) dibujarOrbitas(cuerpos, modelLoc, colorLoc);
    dibujarCuerpos(cuerpos, modelLoc, colorLoc);
}

void limpiar(std::vector<CuerpoCeleste*>& cuerpos) {
    for (CuerpoCeleste* c : cuerpos) free(c);
    glfwTerminate();
}