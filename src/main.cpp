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

void processInput(GLFWwindow* window, std::vector<Planeta*>& planetas, std::vector<Satelite*>& satelites);


// Configuración ventana
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

// para escoger el planeta que vamos a mirar dentro de la funcion processInput
Planeta* planetaObjetivo = nullptr;
Satelite* sateliteObjetivo = nullptr;
float distanciaCamara = 2.0f;
glm::vec3 direccionCamara = glm::normalize(glm::vec3(0.0f, 0.4f, 1.0f));
bool vistaDesdeLuna = false;
bool vistaDesdeTierra = false;

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
    std::vector<Satelite*> satelites = inicializarSatelites(VAO_esfera);

    // Loop principal
    while (!glfwWindowShouldClose(window)) {
        processInput(window, planetas, satelites);

        actualizarMovimiento(planetas);
        actualizarMovimientoSat(satelites,planetas[3]);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glm::mat4 view;

        if (planetaObjetivo != nullptr || sateliteObjetivo != nullptr) {
            glm::vec3 target;

            if (planetaObjetivo != nullptr) {
                target = glm::vec3(
                    planetaObjetivo->posicion[0],
                    planetaObjetivo->posicion[1],
                    planetaObjetivo->posicion[2]
                );
            }
            else {
                target = glm::vec3(
                    sateliteObjetivo->posicion[0],
                    sateliteObjetivo->posicion[1],
                    sateliteObjetivo->posicion[2]
                );
            }

            glm::vec3 cameraPos;

            // Tuve que añadir esto porque desde la luna la vista es distinta:
            if (vistaDesdeLuna) {
                cameraPos = glm::vec3(
                    satelites[0]->posicion[0],
                    satelites[0]->posicion[1],
                    satelites[0]->posicion[2]
                );
            }
            else if (vistaDesdeTierra) {
                cameraPos = glm::vec3(
                    planetas[3]->posicion[0],
                    planetas[3]->posicion[1],
                    planetas[3]->posicion[2]
                );
            }
            else {
                cameraPos = target + direccionCamara * distanciaCamara;
            }

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

        dibujarOrbitas(planetas, modelLoc, colorLoc);
        dibujarPlanetas(planetas, modelLoc, colorLoc);
        dibujarSatelites(satelites, modelLoc, colorLoc);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void processInput(GLFWwindow* window, std::vector<Planeta*>& planetas, std::vector<Satelite*>& satelites){
    // esta velocidad controla lo rapido que giramos con las flechas
    float velocidadAngular = 0.02f;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // con la tecla del 1 enfocamos marte
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
        planetaObjetivo = planetas[4];
        sateliteObjetivo = nullptr;
        distanciaCamara = 0.4f;
        vistaDesdeLuna = false; // si no, después de presionar 2 y darle a 1, se enfoca a marte desde la luna y eso no lo queremos
        vistaDesdeTierra = false;
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        planetaObjetivo = planetas[3];
        sateliteObjetivo = nullptr;
        vistaDesdeLuna = true;
        vistaDesdeTierra = false;
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        planetaObjetivo = nullptr;
        sateliteObjetivo = satelites[1];
        vistaDesdeLuna = false;
        vistaDesdeTierra = true;
    }

    // tecla 4 para volver a la camara inicial
    if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS){
        planetaObjetivo = nullptr;
        sateliteObjetivo = nullptr;
        distanciaCamara = 2.0f;
        vistaDesdeLuna = false;
        vistaDesdeTierra = false;
    }
}