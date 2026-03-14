#ifndef PLANETAS_H
#define PLANETAS_H

#include <stdio.h>
#include <glm/glm.hpp>

typedef struct Planeta {
    char nombre[20];
    float tamanho; // puede ser el radio o el diámetro

    float distanciaAlSol;

    float velocidadTraslacion; // velocidad alrededor del sol
    float velocidadRotacion;
    
    float inclinacion; // (de la órbita)
    
    float posicion[3]; // vector de 3 posiciones: x, y, z
    glm::vec3 color; // esto es para el color del planeta pero cuando tengamos
                     // las texturas de los planetas ya no creo que haga falta

}Planeta;

/* La usaremos para crear todos los planetas sin tener que meter los datos en el struct manualmente*/
Planeta* crearPlaneta(char nombre[20], float tamanho, float distanciaAlSol, float velocidadTraslacion, float velocidadRotacion, float inclinacion, float posicion[3]);

#endif