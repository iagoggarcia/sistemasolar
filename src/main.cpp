#include <glad.h>
#include <GLFW/glfw3.h>

// estas dos hacen falta para el model y demás:
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "lecturaShader_0_9.h"
#include "esfera.h"
#include "planetas.h"
#include "satelites.h"
#include "camara.h"


// Configuración ventana
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

float deltaTime = 0.0f;
float ultimoFrame = 0.0f;

// shaders
extern GLuint setShaders(const char* nVertx, const char* nFrag);
GLuint shaderProgram;

void reescalarVentana(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// definimos los vaos que guardan y usan los vértices que nos dieron en otra
// práctica para luego usarlos al crear los planetas
GLuint VAO_esfera;
GLuint VBO_esfera;


void openGlInit() {
    glClearDepth(1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // color de fondo de la ventana
    glEnable(GL_DEPTH_TEST);
}

int main() {
    // Inicializar GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Crear ventana
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Sistema Solar", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Error creando ventana" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, reescalarVentana);

    glfwMakeContextCurrent(window);

    // Inicializar GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Error inicializando GLAD" << std::endl;
        return -1;
    }

    // Configuración OpenGL
    openGlInit();

    // Cargar shaders
    shaderProgram = setShaders("shaders/shader.vert", "shaders/shader.frag");

    // esto sirve para mandar la matriz de transformación al shader
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    // esto es para saber a dónde mandar el color de cada planeta
    GLuint colorLoc = glGetUniformLocation(shaderProgram, "color");

    // estas dos localizaciones nuevas son para pasarle al shader
    // la camara y la perspectiva con la que vamos a ver la escena
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    crearEsfera(VAO_esfera, VBO_esfera);
    // Aquí usamos la función que habías hecho ayer para guardar los planetas en el vector y usarlos en el bucle de debajo (para dibujarlos)
    std::vector<Planeta*> planetas = inicializarPlanetas(VAO_esfera);
    std::vector<Satelite*> satelites = inicializarSatelites(VAO_esfera);

    // Loop principal
    while (!glfwWindowShouldClose(window)) {
        float tiempoActual = glfwGetTime();
        deltaTime = tiempoActual - ultimoFrame;
        ultimoFrame = tiempoActual;

        processInput(window, planetas, satelites);

        actualizarMovimiento(planetas, deltaTime);
        actualizarMovimientoSat(satelites,planetas[3], deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glm::mat4 view = obtenerVista(planetas, satelites);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glm::mat4 projection = glm::perspective(
            glm::radians(60.0f),
            (float)width / (float)height,
            0.1f,
            100.0f
        );

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        dibujarOrbitas(planetas, modelLoc, colorLoc);
        dibujarPlanetas(planetas, modelLoc, colorLoc);
        dibujarSatelites(satelites, modelLoc, colorLoc);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}