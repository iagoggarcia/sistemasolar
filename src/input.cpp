#include "input.h"
#include "camara.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void cerrarVentanaSiEsc(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

void procesarTeclaOrbitas(GLFWwindow* window) {
    bool teclaOPulsadaAhora = (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS);

    if (teclaOPulsadaAhora && !teclaOPulsadaAntes) {
        mostrarOrbitas = !mostrarOrbitas;
    }

    teclaOPulsadaAntes = teclaOPulsadaAhora;
}

void procesarTeclasVelocidad(GLFWwindow* window) {
    static bool teclaSLibre = true;
    static bool teclaBLibre = true;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && teclaSLibre) {
        factorVelocidad += 0.25f;
        teclaSLibre = false;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
        teclaSLibre = true;
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && teclaBLibre) {
        factorVelocidad -= 0.25f;
        if (factorVelocidad < 0.10f)
            factorVelocidad = 0.10f;

        teclaBLibre = false;
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE) {
        teclaBLibre = true;
    }
}

void procesarTeclasEnfoque(GLFWwindow* window, std::vector<CuerpoCeleste*>& cuerpos) {
    
    // ------------------ PARA LOS PlANETAS ------------------
    
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        enfocarMarte(cuerpos);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        enfocarTierraDesdeLuna(cuerpos);
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        enfocarIssDesdeTierra(cuerpos);
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        ponerCamaraGeneral();
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        enfocarPlanetaDesdeTierra(cuerpos[1]);
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
        enfocarPlanetaDesdeTierra(cuerpos[2]);
    }
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
        enfocarPlanetaDesdeTierra(cuerpos[4]);
    }
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
        enfocarPlanetaDesdeTierra(cuerpos[5]);
    }
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
        enfocarPlanetaDesdeTierra(cuerpos[6]);
    }
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
        enfocarPlanetaDesdeTierra(cuerpos[7]);
    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
        enfocarPlanetaDesdeTierra(cuerpos[8]);
    }

    // ------------------ PARA LOS SATÉLITES (luna e ISS) ------------------

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        enfocarSateliteDesdeTierra(cuerpos[9]); // luna
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        enfocarSateliteDesdeTierra(cuerpos[10]); // estación espacial
    }
}

void procesarMovimientoCamaraGeneral(GLFWwindow* window, float deltaTime) {

    // solo mover si estamos en cámara general
    if (cuerpoObjetivo != nullptr)
        return;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        avanzarCamaraGeneral(deltaTime);

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        retrocederCamaraGeneral(deltaTime);

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        girarCamaraGeneralIzquierda(deltaTime);

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        girarCamaraGeneralDerecha(deltaTime);
}

void processInput(GLFWwindow* window, std::vector<CuerpoCeleste*>& cuerpos, float deltaTime) {
    cerrarVentanaSiEsc(window);
    procesarMovimientoCamaraGeneral(window, deltaTime);
    procesarTeclasEnfoque(window, cuerpos);
    procesarTeclaOrbitas(window);
    procesarTeclasVelocidad(window);
}