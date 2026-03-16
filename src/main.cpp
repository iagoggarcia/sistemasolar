#include <glad.h>
#include <GLFW/glfw3.h>

// estas dos hacen falta para el model y demás:
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "lecturaShader_0_9.h"
#include "esfera.h"
#include "planetas.h"

void processInput(GLFWwindow* window, std::vector<Planeta*>& planetas);


// Configuración ventana
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

// para escoger el planeta que vamos a mirar dentro de la funcion processInput
Planeta* planetaObjetivo = nullptr;
float distanciaCamara = 2.0f;
glm::vec3 direccionCamara = glm::normalize(glm::vec3(0.0f, 0.4f, 1.0f));

// shaders
extern GLuint setShaders(const char* nVertx, const char* nFrag);
GLuint shaderProgram;

void reescalarVentana(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

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

    crearEsfera();
    // Aquí usamos la función que habías hecho ayer para guardar los planetas en el vector y usarlos en el bucle de debajo (para dibujarlos)
    std::vector<Planeta*> planetas = inicializarPlanetas(VAO_esfera);

    // Loop principal
    while (!glfwWindowShouldClose(window)) {
        processInput(window, planetas);

        actualizarMovimiento(planetas);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glm::mat4 view;

        if (planetaObjetivo != nullptr) {
            glm::vec3 target(
                planetaObjetivo->posicion[0],
                planetaObjetivo->posicion[1],
                planetaObjetivo->posicion[2]
            );

            glm::vec3 cameraPos = target + direccionCamara * distanciaCamara;

            view = glm::lookAt(
                cameraPos,
                target,
                glm::vec3(0.0f, 1.0f, 0.0f)
            );
        }
        else {
            view = glm::lookAt(
                glm::vec3(10.0f, 3.0f, 8.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );
        }

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

        dibujarPlanetas(planetas, modelLoc, colorLoc);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void processInput(GLFWwindow* window, std::vector<Planeta*>& planetas){
    // esta velocidad controla lo rapido que giramos con las flechas
    float velocidadAngular = 0.02f;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // con la tecla del 1 enfocamos marte
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
        planetaObjetivo = planetas[4];
        distanciaCamara = 0.4f;
    }

    if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS){
        planetaObjetivo = nullptr;
        distanciaCamara = 2.0f;
    }
}