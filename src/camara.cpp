#include "camara.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Planeta* planetaObjetivo = nullptr;
Satelite* sateliteObjetivo = nullptr;
float distanciaCamara = 2.0f;
glm::vec3 direccionCamara = glm::normalize(glm::vec3(0.0f, 0.4f, 1.0f));
bool vistaDesdeLuna = false;
bool vistaDesdeTierra = false;
glm::vec3 posicionCamaraGeneral = glm::vec3(10.0f, 3.0f, 8.0f);
glm::vec3 direccionCamaraGeneral = glm::normalize(glm::vec3(-10.0f, -3.0f, -8.0f));
glm::vec3 upCamaraGeneral = glm::vec3(0.0f, 1.0f, 0.0f);

bool mostrarOrbitas = true; // empezamos dibujándolas y luego si se toca la tecla '' se quitan
bool teclaOPulsadaAntes = false; // si no metía esta variable daba parpadeos para pintar/despintar las órbitas

float factorVelocidad = 1.0f; // para poder acelerar/disminuir la velocidad del sistema


glm::mat4 obtenerVista(std::vector<Planeta*>& planetas, std::vector<Satelite*>& satelites) {
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
            posicionCamaraGeneral,
            posicionCamaraGeneral + direccionCamaraGeneral,
            upCamaraGeneral
        );
    }

    return view;
}

void processInput(GLFWwindow* window, std::vector<Planeta*>& planetas, std::vector<Satelite*>& satelites, float deltaTime) {
    float velocidadAngular = 1.8f * deltaTime;
    float velocidadMovimiento = 6.0f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (planetaObjetivo == nullptr && sateliteObjetivo == nullptr) {
        glm::vec3 direccionHorizontal = glm::normalize(glm::vec3(direccionCamaraGeneral.x, 0.0f, direccionCamaraGeneral.z));

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            posicionCamaraGeneral += direccionHorizontal * velocidadMovimiento;

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            posicionCamaraGeneral -= direccionHorizontal * velocidadMovimiento;

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            glm::mat4 rot = glm::rotate(glm::mat4(1.0f), velocidadAngular, glm::vec3(0.0f, 1.0f, 0.0f));
            direccionCamaraGeneral = glm::normalize(glm::vec3(rot * glm::vec4(direccionCamaraGeneral, 0.0f)));
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            glm::mat4 rot = glm::rotate(glm::mat4(1.0f), -velocidadAngular, glm::vec3(0.0f, 1.0f, 0.0f));
            direccionCamaraGeneral = glm::normalize(glm::vec3(rot * glm::vec4(direccionCamaraGeneral, 0.0f)));
        }
    }

    // enfocar marte
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
        planetaObjetivo = planetas[4];
        sateliteObjetivo = nullptr;
        distanciaCamara = 0.4f;
        vistaDesdeLuna = false;
        vistaDesdeTierra = false;
    }

    // enfocar tierra desde la luna
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        planetaObjetivo = planetas[3];
        sateliteObjetivo = nullptr;
        vistaDesdeLuna = true;
        vistaDesdeTierra = false;
    }

    // enfocar estación espacial desde la tierra
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        planetaObjetivo = nullptr;
        sateliteObjetivo = satelites[1];
        vistaDesdeLuna = false;
        vistaDesdeTierra = true;
    }

    // cámara general/principal de todo el sistema
    if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS){
        planetaObjetivo = nullptr;
        sateliteObjetivo = nullptr;
        distanciaCamara = 2.0f;
        vistaDesdeLuna = false;
        vistaDesdeTierra = false;
    }

    // enfocar mercurio desde la tierra
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        planetaObjetivo = planetas[1];
        sateliteObjetivo = nullptr;
        vistaDesdeLuna = false;
        vistaDesdeTierra = true;
    }

    // enfocar venus desde la tierra
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
        planetaObjetivo = planetas[2];
        sateliteObjetivo = nullptr;
        vistaDesdeLuna = false;
        vistaDesdeTierra = true;
    }

    // enfocar marte desde la tierra
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
        planetaObjetivo = planetas[4];
        sateliteObjetivo = nullptr;
        vistaDesdeLuna = false;
        vistaDesdeTierra = true;
    }

    // enfocar luna desde la tierra
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
        planetaObjetivo = nullptr;
        sateliteObjetivo = satelites[0];
        vistaDesdeLuna = false;
        vistaDesdeTierra = true;
    }

    // enfocar júpiter desde la tierra
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
        planetaObjetivo = planetas[5];
        sateliteObjetivo = nullptr;
        vistaDesdeLuna = false;
        vistaDesdeTierra = true;
    }

    // enfocar saturno desde la tierra
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
        planetaObjetivo = planetas[6];
        sateliteObjetivo = nullptr;
        vistaDesdeLuna = false;
        vistaDesdeTierra = true;
    }

    // enfocar urano desde la tierra
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        planetaObjetivo = planetas[7];
        sateliteObjetivo = nullptr;
        vistaDesdeLuna = false;
        vistaDesdeTierra = true;
    }

    // enfocar neptuno desde la tierra
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
        planetaObjetivo = planetas[8];
        sateliteObjetivo = nullptr;
        vistaDesdeLuna = false;
        vistaDesdeTierra = true;
    }

    bool teclaOPulsadaAhora = (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS);

    if (teclaOPulsadaAhora && !teclaOPulsadaAntes) {
        mostrarOrbitas = !mostrarOrbitas;
    }

    teclaOPulsadaAntes = teclaOPulsadaAhora;

    // Bloque para subir (tecla s) y bajar (tecla b) la velocidad
    bool teclaSLibre = true;
    bool teclaBLibre = true;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && teclaSLibre) {
        factorVelocidad += 0.10f;
        teclaSLibre = false;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
        teclaSLibre = true;
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && teclaBLibre) {
        factorVelocidad -= 0.10f;
        if (factorVelocidad < 0.10f) factorVelocidad = 0.10f; // ponemos un límite para que no se pare
        teclaBLibre = false;
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE) {
        teclaBLibre = true;
    }
}