#ifndef CUERPO_H
#define CUERPO_H

#include <glm/glm.hpp>
#include <glad.h>
#include <vector>

struct CuerpoCeleste {
    char nombre[20];
    float tamanho;

    float distanciaAlPadre; // antes era distanciaAlSol o distanciaTierra
    float velocidadTraslacion;
    float velocidadRotacion;

    float anguloTraslacion;
    float anguloRotacion;

    float posicion[3];
    glm::vec3 color;

    GLuint VAO;
    GLuint VAOorbita;
    int numVerticesOrbita;
    float inclinacionOrbita;

    GLuint textura1;
    GLuint textura2;
    bool multitextura;

    CuerpoCeleste* padre; // va a ser nulo si orbita el sol
};

void actualizarMovimiento(std::vector<CuerpoCeleste*>& cuerpos, float deltaTime, float factorVelocidad);
std::vector<CuerpoCeleste*> inicializarCuerpos(GLuint VAO_esfera);
CuerpoCeleste* crearCuerpo(const char* nombre, float tamanho, float velTras, float velRot, float distancia, float inclinacionOrbita, glm::vec3 color, GLuint VAO, CuerpoCeleste* padre);
std::vector<float> crearVerticesOrbita(float radio);
GLuint crearVAOorbita(float radio);
void dibujarOrbitas(std::vector<CuerpoCeleste*>& cuerpos, GLuint modelLoc, GLuint objectColorLoc);
void dibujarCuerpos(std::vector<CuerpoCeleste*>& cuerpos, GLuint modelLoc, GLuint objectColorLoc, GLuint esSolLoc);

#endif