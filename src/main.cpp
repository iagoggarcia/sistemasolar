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

// estas variables las usamos para controlar la camara orbital:
// horz para girar alrededor del sistema en horizontal,
// vert para subir o bajar el angulo de vision
// y dist_zoom para acercarnos o alejarnos
float horz = 0.0f;
float vert = 0.3f;
float dist_zoom = 2.5f;

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

    // estas dos localizaciones nuevas son para pasarle al shader
    // la camara y la perspectiva con la que vamos a ver la escena
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    crearEsfera();
    // Aquí usamos la función que habías hecho ayer para guardar los planetas en el vector y usarlos en el bucle de debajo (para dibujarlos)
    std::vector<Planeta*> planetas = inicializarPlanetas(VAO_esfera);

    // Loop principal
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // limpiar pantalla
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // aqui calculamos donde esta la camara en cada momento
        // a partir del giro horizontal, el vertical y la distancia
        glm::vec3 cameraPos;
        cameraPos.x = dist_zoom * cos(vert) * cos(horz);
        cameraPos.y = dist_zoom * sin(vert);
        cameraPos.z = dist_zoom * cos(vert) * sin(horz);

        // con lookAt construimos la vista de la camara:
        // le decimos donde estamos, a donde miramos y cual es el arriba
        // en este caso miramos siempre al centro del sistema
        glm::mat4 view = glm::lookAt(
            cameraPos,
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        // esta matriz hace que la escena se vea con perspectiva
        // para que lo lejano se vea mas pequeño y no quede todo plano
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            (float)SCR_WIDTH / (float)SCR_HEIGHT,
            0.1f,
            100.0f
        );

        // mandamos al shader la vista de la camara y la perspectiva
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

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
    // esta velocidad controla lo rapido que giramos con las flechas
    float velocidadAngular = 0.02f;

    // esta controla lo rapido que nos acercamos o alejamos
    float velocidadZoom = 0.03f;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // con izquierda y derecha giramos alrededor del sistema
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        horz -= velocidadAngular;

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        horz += velocidadAngular;

    // con arriba y abajo cambiamos la altura desde la que lo estamos viendo
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        vert += velocidadAngular;

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        vert -= velocidadAngular;

    // con W y S hacemos zoom
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        dist_zoom -= velocidadZoom;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        dist_zoom += velocidadZoom;

    // limitamos el angulo vertical para que la camara no se coloque
    // en una posicion rara justo por encima o por debajo
    if (vert > 1.5f) vert = 1.5f;
    if (vert < -1.5f) vert = -1.5f;

    // tambien limitamos el zoom para no meternos demasiado dentro
    // ni alejarnos tanto que deje de tener sentido
    if (dist_zoom < 0.3f) dist_zoom = 0.3f;
    if (dist_zoom > 20.0f) dist_zoom = 20.0f;
}