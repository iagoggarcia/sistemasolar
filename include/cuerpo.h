#ifndef CUERPO_H
#define CUERPO_H

#include <glm/glm.hpp>
#include <glad.h>
#include <vector>

struct CuerpoCeleste {
    char nombre[20];
    float tamanho;

    float distanciaAlPadre;
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

    CuerpoCeleste* padre;
};

void actualizarMovimiento(std::vector<CuerpoCeleste*>& cuerpos, float deltaTime, float factorVelocidad);
std::vector<CuerpoCeleste*> inicializarCuerpos(GLuint VAO_esfera);
CuerpoCeleste* crearCuerpo(const char* nombre, float tamanho, float velTras, float velRot, float distancia, float inclinacionOrbita, glm::vec3 color, GLuint VAO, CuerpoCeleste* padre);
std::vector<float> crearVerticesOrbita(float radio);
GLuint crearVAOorbita(float radio);

// CAMBIO IMPORTANTE: ya no reciben uniforms desde fuera
void dibujarOrbitas(std::vector<CuerpoCeleste*>& cuerpos);
void dibujarCuerpos(std::vector<CuerpoCeleste*>& cuerpos);

#endif