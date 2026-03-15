#include <glad/glad.h>
#include <GLFW/glfw3.h>

// estas dos hacen falta para el model y demás:
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "lecturaShader_0_9.h"
#include "esfera.h"
#include "planetas.h"

void processInput(GLFWwindow* window);

// Configuración ventana
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// shaders
extern GLuint setShaders(const char* nVertx, const char* nFrag);
GLuint shaderProgram;

// definimos los vaos que guardan y usan los vértices que nos dieron en otra
// práctica para luego usarlos al crear los planetas
unsigned int VAO_esfera;
unsigned int VBO_esfera;

void crearEsfera() {
    glGenVertexArrays(1, &VAO_esfera);
    glGenBuffers(1, &VBO_esfera);

    glBindVertexArray(VAO_esfera);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_esfera);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_esfera), vertices_esfera, GL_STATIC_DRAW);

    // normales (los 3 primeros valores float)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    // textura (los 2 siguientes valores float)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // posición (los 3 últimos valores float)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

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

    crearEsfera();
    // Aquí usamos la función que habías hecho ayer para guardar los planetas en el vector y usarlos en el bucle de debajo (para dibujarlos)
    std::vector<Planeta*> planetas = inicializarPlanetas(VAO_esfera);

    // Loop principal
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // limpiar pantalla
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        dibujarPlanetas(planetas, modelLoc, colorLoc);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}