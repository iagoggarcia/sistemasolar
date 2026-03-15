#include <stdio.h>
#include <stdlib.h>
#include <planetas.h>
#include <string.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Planeta* crearPlaneta(const char* nombre, float tamanho, float distancia, float velTras, float velRot, glm::vec3 color, GLuint VAO) {

    Planeta* planeta = (Planeta*)malloc(sizeof(Planeta));

    strcpy(planeta->nombre, nombre);

    planeta->tamanho = tamanho;
    planeta->distanciaAlSol = distancia;

    planeta->velocidadTraslacion = velTras;
    planeta->velocidadRotacion = velRot;

    planeta->anguloRotacion = 0.0f;
    planeta->anguloTraslacion = 0.0f;

    planeta->color = color;
    planeta->VAO = VAO;

    // Ponemos las coordenadas a 0 al principio, porq luego se van actualizando frame a frame con el movimiento
    planeta->posicion[0] = 0;
    planeta->posicion[1] = 0;
    planeta->posicion[2] = 0;

    return planeta;
}

std::vector<Planeta*> inicializarPlanetas(GLuint VAO_esfera){

    //aqui inicializo el vector en el que voy a meter los planetas
    std::vector<Planeta*> planetas;

    //lo que hace pushback es meter al final del vector lo que le pases, asi que le metemos el crear planeta con datos aproximados que me dio chati
    //no tengo puta idea de la inclinacion y esas cosas asi que de momento con dibujarlos mas o menos llega
    //las distancias y tamaños no son ni parecidos a los reales pero bueno, son mas o menos orientativos para que se pueda apreciar el sistema entero, que si no la tierra quedaria enana con el sol
    planetas.push_back(crearPlaneta("Sol",      0.08f, 0.0f, 0.0f, 0.0f, glm::vec3(1.0f, 0.8f, 0.0f), VAO_esfera));
    planetas.push_back(crearPlaneta("Mercurio", 0.015f, 2.0f, 0.0f, 0.0f, glm::vec3(0.6f, 0.6f, 0.6f), VAO_esfera));
    planetas.push_back(crearPlaneta("Venus",    0.02f,  3.0f, 0.0f, 0.0f, glm::vec3(0.9f, 0.7f, 0.2f), VAO_esfera));
    planetas.push_back(crearPlaneta("Tierra",   0.02f,  4.0f, 0.0f, 0.0f, glm::vec3(0.2f, 0.4f, 1.0f), VAO_esfera));
    planetas.push_back(crearPlaneta("Marte",    0.018f, 5.0f, 0.0f, 0.0f, glm::vec3(0.8f, 0.3f, 0.1f), VAO_esfera));
    planetas.push_back(crearPlaneta("Júpiter",  0.04f,  6.5f, 0.0f, 0.0f, glm::vec3(0.8f, 0.6f, 0.4f), VAO_esfera));
    planetas.push_back(crearPlaneta("Saturno",  0.035f, 8.0f, 0.0f, 0.0f, glm::vec3(0.9f, 0.8f, 0.5f), VAO_esfera));
    planetas.push_back(crearPlaneta("Urano",    0.03f,  9.5f, 0.0f, 0.0f, glm::vec3(0.5f, 0.8f, 1.0f), VAO_esfera));
    planetas.push_back(crearPlaneta("Neptuno",  0.03f, 11.0f, 0.0f, 0.0f, glm::vec3(0.2f, 0.3f, 0.9f), VAO_esfera));
    
    //devolvemos el vector y ya tenemos todos los datos para dibujar luego en el main recorriendo el vector y reutilizando la esfera que ya tenemos y dandole los valores de tamaño y tal que le metimos
    return planetas;
}

void dibujarPlanetas(std::vector<Planeta*>& planetas, GLuint modelLoc, GLuint colorLoc) {
    for (Planeta* planeta : planetas) {
            // matriz modelo
            glm::mat4 model = glm::mat4(1.0f);

            // mover el planeta a su distancia del sol (por ahora 'y' y 'z' los dejamos como están q no tengo ni idea de los valores)
            model = glm::translate(model, glm::vec3(planeta->distanciaAlSol * 0.08f, 0.0f, 0.0f));
            
            // Escalamos la esfera base según el tamaño del planeta.
            // La esfera original tiene tamaño 1 y aquí la multiplicamos por planeta->tamanho.
            model = glm::scale(model, glm::vec3(planeta->tamanho));

            // enviar matriz al shader
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            // enviamos el color del planeta
            glUniform3fv(colorLoc, 1, glm::value_ptr(planeta->color));

            // dibujar el planeta con los 1080 vértices de la esfera
            glBindVertexArray(planeta->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 1080);
        }
}