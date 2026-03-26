#ifndef SATELITES_H
#define SATELITES_H

#include <stdio.h>
#include <glm/glm.hpp>
#include <vector>
#include <glad.h>
#include "planetas.h"

typedef struct Satelite{
    char nombre[20];
    float tamanho; // puede ser el radio o el diámetro

    float distanciaTierra;

    float velocidadTraslacion; // velocidad alrededor del planeta que orbita
    float velocidadRotacion;

    float anguloRotacion;
    float anguloTraslacion;
    
    float posicion[3]; // vector de 3 posiciones: x, y, z
    glm::vec3 color; // esto es para el color del planeta pero cuando tengamos
                     // las texturas de los planetas ya no creo que haga falta

    GLuint VAO; // puntero al VAO

}Satelite;

Satelite *crearSatelite(const char *nombre, float tamanho, float py, float pz, float distancia, float velTras, float velRot, glm::vec3 color, GLuint VAO);

std::vector<Satelite*> inicializarSatelites(GLuint VAO_esfera);

/* Función que coge el vector con todos los satelites y sus datos y los dibuja. La usamos en el main pero
   la definimos aquí para que el main quede más limpio.
*/
void dibujarSatelites(std::vector<Satelite*>& satelites, GLuint modelLoc, GLuint colorLoc);

void actualizarMovimientoSat(std::vector<Satelite*>& satelites, Planeta* tierra, float deltaTime);

#endif