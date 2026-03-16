#ifndef PLANETAS_H
#define PLANETAS_H

#include <stdio.h>
#include <glm/glm.hpp>
#include <vector>
#include <glad.h>

typedef struct Planeta {
    char nombre[20];
    float tamanho; // puede ser el radio o el diámetro

    float distanciaAlSol;

    float velocidadTraslacion; // velocidad alrededor del sol
    float velocidadRotacion;

    float anguloRotacion;
    float anguloTraslacion;
    
    float posicion[3]; // vector de 3 posiciones: x, y, z
    glm::vec3 color; // esto es para el color del planeta pero cuando tengamos
                     // las texturas de los planetas ya no creo que haga falta

    GLuint VAO; // puntero al VAO

}Planeta;

/* La usaremos para crear todos los planetas sin tener que meter los datos en el struct manualmente*/
Planeta* crearPlaneta(const char* nombre, float tamanho, float distancia, float velTras, float velRot, glm::vec3 color, GLuint VAO);

/* Función que usamos para definir y dar valores iniciales (específicos) a los planetas que conforman el sistema.
    Argumentos:
    - GLuint VAO_esfera -> sirve para usar la esfera de 1080 vértices definida en esfera.h
*/
std::vector<Planeta*> inicializarPlanetas(GLuint VAO_esfera);

/* Función que coge el vector con todos los planetas y sus datos y los dibuja. La usamos en el main pero
   la definimos aquí para que el main quede más limpio.
*/
void dibujarPlanetas(std::vector<Planeta*>& planetas, GLuint modelLoc, GLuint colorLoc);

void actualizarMovimiento(std::vector<Planeta*>& planetas);

#endif