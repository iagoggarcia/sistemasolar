#include <stdio.h>
#include <stdlib.h>
#include <planetas.h>
#include <string.h>

Planeta* crearPlaneta(char nombre[20], float tamanho, float distanciaAlSol, float velocidadTraslacion, float velocidadRotacion, float inclinacion, glm::vec3 color) {
    
    Planeta* planetaNuevo = (Planeta*)malloc(sizeof(Planeta));
    if (!planetaNuevo) {
        printf("Error en la asignación de memoria para un planeta");
        exit(1);
    }

    strncpy(planetaNuevo->nombre, nombre, sizeof(planetaNuevo->nombre)); // guardamos el nombre en el struct

    // Ahora guardamos todos los demás datos (numéricos):
    planetaNuevo->tamanho = tamanho;
    planetaNuevo->velocidadRotacion = velocidadRotacion;
    planetaNuevo->velocidadTraslacion = velocidadTraslacion;
    planetaNuevo->distanciaAlSol = distanciaAlSol;
    planetaNuevo->inclinacion = inclinacion;

    // Y el color:
    planetaNuevo->color = color;

    // E inicializamos la posición a una predeterminada (todo 0's):
    for (int i = 0; i < 3; i++) {
        planetaNuevo->posicion[i] = 0.0f;
    }


    return planetaNuevo;
}