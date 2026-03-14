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

//funcion para inicializar los planetas en un vector de planetas
std::vector<Planeta*> inicializarPlanetas(){

    //aqui inicializo el vector en el qeu voy a meter los planetas
    std::vector<Planeta*> planetas;

    //lo que hace pushback es meter al final del vector lo que le pases, asi que le metemos el crear planeta con datos aproximados que me dio chati
    //no tengo puta idea de la inclinacion y esas cosas asi que de momento con dibujarlos mas o menos llega
    //las distancias y tamaños no son ni parecidos a los reales pero bueno, son mas o menos orientativos para que se pueda apreciar el sistema entero, que si no la tierra quedaria enana con el sol
    planetas.push_back(crearPlaneta("Sol",      2.5f,  0.0f, 0.0f, 0.0f, 0.0f, glm::vec3(1.0f, 0.8f, 0.0f)));
    planetas.push_back(crearPlaneta("Mercurio", 0.2f,  4.0f, 0.0f, 0.0f, 0.0f, glm::vec3(0.6f, 0.6f, 0.6f)));
    planetas.push_back(crearPlaneta("Venus",    0.35f, 6.0f, 0.0f, 0.0f, 0.0f, glm::vec3(0.9f, 0.7f, 0.2f)));
    planetas.push_back(crearPlaneta("Tierra",   0.37f, 8.0f, 0.0f, 0.0f, 0.0f, glm::vec3(0.2f, 0.4f, 1.0f)));
    planetas.push_back(crearPlaneta("Marte",    0.28f, 10.0f, 0.0f, 0.0f, 0.0f, glm::vec3(0.8f, 0.3f, 0.1f)));
    planetas.push_back(crearPlaneta("Jupiter",  0.9f,  14.0f, 0.0f, 0.0f, 0.0f, glm::vec3(0.8f, 0.6f, 0.4f)));
    planetas.push_back(crearPlaneta("Saturno",  0.75f, 18.0f, 0.0f, 0.0f, 0.0f, glm::vec3(0.9f, 0.8f, 0.5f)));
    planetas.push_back(crearPlaneta("Urano",    0.55f, 22.0f, 0.0f, 0.0f, 0.0f, glm::vec3(0.5f, 0.8f, 1.0f)));
    planetas.push_back(crearPlaneta("Neptuno",  0.53f, 26.0f, 0.0f, 0.0f, 0.0f, glm::vec3(0.2f, 0.3f, 0.9f)));
    
    //devolvemos el vector y ya tenemos todos los datos para dibujar luego en el main recorriendo el vector y reutilizando la esfera que ya tenemos y dandole los valores de tamaño y tal que le metimos
    return planetas;
}