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

// shader programs definidos en main.cpp
extern GLuint shaderIluminacionProgram;
extern GLuint shaderSimpleProgram;

bool luzEncendida = true;

// inicializa GLFW, GLAD y la ventana
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
    glfwSwapInterval(1); // sincroniza con el refresco de pantalla

    // ajusta el viewport si cambia el tamaño de la ventana
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int w, int h) {
        glViewport(0, 0, w, h);
    });

    // carga funciones de OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Error inicializando GLAD" << std::endl;
        return nullptr;
    }

    glClearDepth(1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST); // activa el test de profundidad

    return window;
}

// actualiza tiempos y movimiento
void actualizarEstado(GLFWwindow* window, std::vector<CuerpoCeleste*>& cuerpos) {
    float tiempoActual = glfwGetTime();
    deltaTime   = tiempoActual - ultimoFrame; // tiempo entre frames
    ultimoFrame = tiempoActual;

    processInput(window, cuerpos, deltaTime); // procesa input usuario
    actualizarMovimiento(cuerpos, deltaTime, factorVelocidad); // mueve planetas
}

// renderiza toda la escena
void renderizar(std::vector<CuerpoCeleste*>& cuerpos, GLFWwindow* window) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // matriz de vista (cámara)
    glm::mat4 view = obtenerVista(cuerpos);

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    // matriz de proyección en perspectiva
    glm::mat4 projection = glm::perspective(
        glm::radians(60.0f),
        (float)w / (float)h,
        0.1f,
        100.0f
    );

    // dibuja órbitas con shader simple
    if (mostrarOrbitas) {
        glUseProgram(shaderSimpleProgram);

        GLuint viewLoc       = glGetUniformLocation(shaderSimpleProgram, "view");
        GLuint projectionLoc = glGetUniformLocation(shaderSimpleProgram, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        dibujarOrbitas(cuerpos);
    }

    // usamos shader con iluminación para los cuerpos
    glUseProgram(shaderIluminacionProgram);

    GLuint viewLoc           = glGetUniformLocation(shaderIluminacionProgram, "view");
    GLuint projectionLoc     = glGetUniformLocation(shaderIluminacionProgram, "projection");
    GLuint lightColorLoc     = glGetUniformLocation(shaderIluminacionProgram, "lightColor");
    GLuint lightPosLoc       = glGetUniformLocation(shaderIluminacionProgram, "lightPos");
    GLuint luzEncendidaLoc   = glGetUniformLocation(shaderIluminacionProgram, "luzEncendida");
    GLuint textura1Loc       = glGetUniformLocation(shaderIluminacionProgram, "texture1");
    GLuint textura2Loc       = glGetUniformLocation(shaderIluminacionProgram, "texture2");
    GLuint viewPosLoc        = glGetUniformLocation(shaderIluminacionProgram, "viewPos");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glUniform3f(lightColorLoc, 2.0f, 2.0f, 2.0f); // intensidad de la luz

    // la luz está en el Sol (primer cuerpo)
    glUniform3f(lightPosLoc,
                cuerpos[0]->posicion[0],
                cuerpos[0]->posicion[1],
                cuerpos[0]->posicion[2]);

    glUniform1i(luzEncendidaLoc, luzEncendida ? 1 : 0); // encender/apagar luz

    glUniform1i(textura1Loc, 0);
    glUniform1i(textura2Loc, 1);

    // calcula posición de cámara para iluminación especular
    glm::vec3 cameraPos;
    if (cuerpoObjetivo != nullptr) {
        glm::vec3 target = obtenerTargetActual();
        cameraPos = obtenerPosicionCamaraActual(cuerpos, target);
    } else {
        cameraPos = posicionCamaraGeneral;
    }

    glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);

    dibujarCuerpos(cuerpos); // dibuja planetas
}

// libera memoria y termina GLFW
void limpiar(std::vector<CuerpoCeleste*>& cuerpos) {
    for (CuerpoCeleste* c : cuerpos) free(c);
    glfwTerminate();
}