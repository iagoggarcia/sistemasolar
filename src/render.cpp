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

bool luzEncendida = true;

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

void renderizar(std::vector<CuerpoCeleste*>& cuerpos, GLuint modelLoc, GLuint objectColorLoc, GLuint viewLoc, GLuint projectionLoc, GLuint lightColorLoc, GLuint lightPosLoc, GLuint esSolLoc, GLuint luzEncendidaLoc, GLFWwindow* window) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    extern GLuint shaderProgram;
    glUseProgram(shaderProgram);

    glUniform3f(lightColorLoc, 2.0f, 2.0f, 2.0f);

    // la luz sale del Sol
    glUniform3f(lightPosLoc,
                cuerpos[0]->posicion[0],
                cuerpos[0]->posicion[1],
                cuerpos[0]->posicion[2]);

    glUniform1i(luzEncendidaLoc, luzEncendida ? 1 : 0);

    GLuint textura1Loc = glGetUniformLocation(shaderProgram, "texture1");
    GLuint textura2Loc = glGetUniformLocation(shaderProgram, "texture2");
    glUniform1i(textura1Loc, 0);
    glUniform1i(textura2Loc, 1);

    glm::mat4 view = obtenerVista(cuerpos);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glm::mat4 projection = glm::perspective(
        glm::radians(60.0f), (float)w / (float)h, 0.1f, 100.0f
    );
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    if (mostrarOrbitas) dibujarOrbitas(cuerpos, modelLoc, objectColorLoc);
    dibujarCuerpos(cuerpos, modelLoc, objectColorLoc, esSolLoc);
}

void limpiar(std::vector<CuerpoCeleste*>& cuerpos) {
    for (CuerpoCeleste* c : cuerpos) free(c);
    glfwTerminate();
}