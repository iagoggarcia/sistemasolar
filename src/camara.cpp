#include "camara.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// objetivos actuales de la cámara
CuerpoCeleste* cuerpoObjetivo = nullptr;

// parámetros de la cámara al enfocar un objetivo
float distanciaCamara = 2.0f;
glm::vec3 direccionCamara = glm::normalize(glm::vec3(0.0f, 0.4f, 1.0f));
bool vistaDesdeLuna = false;
bool vistaDesdeTierra = false;

// estado de la cámara general libre
glm::vec3 posicionCamaraGeneral = glm::vec3(10.0f, 3.0f, 8.0f);
glm::vec3 direccionCamaraGeneral = glm::normalize(glm::vec3(-10.0f, -3.0f, -8.0f));
glm::vec3 upCamaraGeneral = glm::vec3(0.0f, 1.0f, 0.0f);

// estado global de la escena controlado desde la entrada
bool mostrarOrbitas = true; // empezamos dibujándolas y luego si se toca la tecla '' se quitan
bool teclaOPulsadaAntes = false; // si no metía esta variable daba parpadeos para pintar/despintar las órbitas
float factorVelocidad = 1.0f; // para poder acelerar/disminuir la velocidad del sistema

glm::vec3 obtenerTargetActual() {
    return glm::vec3(
        cuerpoObjetivo->posicion[0],
        cuerpoObjetivo->posicion[1],
        cuerpoObjetivo->posicion[2]
    );
}

glm::vec3 obtenerPosicionCamaraActual(std::vector<CuerpoCeleste*>& cuerpos, glm::vec3 target) {
    if (vistaDesdeLuna) {
        return glm::vec3(
            cuerpos[9]->posicion[0],
            cuerpos[9]->posicion[1],
            cuerpos[9]->posicion[2]
        );
    }

    if (vistaDesdeTierra) {
        return glm::vec3(
            cuerpos[3]->posicion[0],
            cuerpos[3]->posicion[1],
            cuerpos[3]->posicion[2]
        );
    }

    return target + direccionCamara * distanciaCamara;
}

glm::mat4 obtenerVista(std::vector<CuerpoCeleste*>& cuerpos) {
    if (cuerpoObjetivo != nullptr) {
        glm::vec3 target = obtenerTargetActual();
        glm::vec3 cameraPos = obtenerPosicionCamaraActual(cuerpos, target);

        return glm::lookAt(cameraPos, target, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    return glm::lookAt(
        posicionCamaraGeneral,
        posicionCamaraGeneral + direccionCamaraGeneral,
        upCamaraGeneral
    );
}

void enfocarMarte(std::vector<CuerpoCeleste*>& cuerpos) {
    cuerpoObjetivo = cuerpos[4];
    distanciaCamara = 0.4f;
    vistaDesdeLuna = false;
    vistaDesdeTierra = false;
}

void enfocarTierraDesdeLuna(std::vector<CuerpoCeleste*>& cuerpos) {
    cuerpoObjetivo = cuerpos[3];
    vistaDesdeLuna = true;
    vistaDesdeTierra = false;
}

void enfocarIssDesdeTierra(std::vector<CuerpoCeleste*>& cuerpos) {
    cuerpoObjetivo = cuerpos[10];
    vistaDesdeLuna = false;
    vistaDesdeTierra = true;
}

void ponerCamaraGeneral() {
    cuerpoObjetivo = nullptr;
    distanciaCamara = 2.0f;
    vistaDesdeLuna = false;
    vistaDesdeTierra = false;
}

void enfocarPlanetaDesdeTierra(CuerpoCeleste* cuerpo) {
    cuerpoObjetivo = cuerpo;
    vistaDesdeLuna = false;
    vistaDesdeTierra = true;
}

void enfocarSateliteDesdeTierra(CuerpoCeleste* cuerpo) {
    cuerpoObjetivo = cuerpo;
    vistaDesdeLuna = false;
    vistaDesdeTierra = true;
}

void avanzarCamaraGeneral(float deltaTime) {
    float velocidadMovimiento = 6.0f * deltaTime;

    glm::vec3 direccionHorizontal = glm::normalize(
        glm::vec3(direccionCamaraGeneral.x, 0.0f, direccionCamaraGeneral.z)
    );

    posicionCamaraGeneral += direccionHorizontal * velocidadMovimiento;
}

void retrocederCamaraGeneral(float deltaTime) {
    float velocidadMovimiento = 6.0f * deltaTime;

    glm::vec3 direccionHorizontal = glm::normalize(
        glm::vec3(direccionCamaraGeneral.x, 0.0f, direccionCamaraGeneral.z)
    );

    posicionCamaraGeneral -= direccionHorizontal * velocidadMovimiento;
}

void girarCamaraGeneralIzquierda(float deltaTime) {
    float velocidadAngular = 1.8f * deltaTime;

    glm::mat4 rot = glm::rotate(
        glm::mat4(1.0f),
        velocidadAngular,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    direccionCamaraGeneral = glm::normalize(
        glm::vec3(rot * glm::vec4(direccionCamaraGeneral, 0.0f))
    );
}

void girarCamaraGeneralDerecha(float deltaTime) {
    float velocidadAngular = 1.8f * deltaTime;

    glm::mat4 rot = glm::rotate(
        glm::mat4(1.0f),
        -velocidadAngular,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    direccionCamaraGeneral = glm::normalize(
        glm::vec3(rot * glm::vec4(direccionCamaraGeneral, 0.0f))
    );
}