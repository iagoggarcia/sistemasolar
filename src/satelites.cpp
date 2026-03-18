#include <stdio.h>
#include <stdlib.h>
#include "satelites.h"
#include <string.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include "planetas.h"

Satelite *crearSatelite(const char *nombre, float tamanho, float py, float pz, float distancia, float velTras, float velRot, glm::vec3 color, GLuint VAO)
{

    Satelite *satelite = (Satelite *)malloc(sizeof(Satelite));

    strcpy(satelite->nombre, nombre);

    satelite->tamanho = tamanho;
    satelite->distanciaTierra = distancia;

    satelite->velocidadTraslacion = velTras;
    satelite->velocidadRotacion = velRot;

    satelite->anguloRotacion = 0.0f;
    satelite->anguloTraslacion = 0.0f;

    satelite->color = color;
    satelite->VAO = VAO;

    // Ponemos las coordenadas a 0 al principio, porq luego se van actualizando frame a frame con el movimiento
    satelite->posicion[0] = distancia;
    satelite->posicion[1] = py;
    satelite->posicion[2] = pz;

    return satelite;
}

std::vector<Satelite *> inicializarSatelites(GLuint VAO_esfera){
    std::vector<Satelite*> satelites;

    satelites.push_back(crearSatelite("Luna", 0.03f, 0.0f, 0.0f, 0.22f, 0.035f, 0.015f, glm::vec3(0.75f, 0.75f, 0.75f), VAO_esfera));
    satelites.push_back(crearSatelite("ISS",  0.01f, 0.0f, 0.0f, 0.12f, 0.075f, 0.022f, glm::vec3(0.9f, 0.9f, 0.95f), VAO_esfera));

    return satelites;
}

void dibujarSatelites(std::vector<Satelite *> &satelites, GLuint modelLoc, GLuint colorLoc)
{
    for (Satelite *satelite : satelites)
    {
        // matriz modelo
        glm::mat4 model = glm::mat4(1.0f);

        // mover el satelite a su distancia a la iterra (por ahora 'y' y 'z' los dejamos como están q no tengo ni idea de los valores)
        model = glm::translate(model, glm::vec3(
            satelite->posicion[0],
            satelite->posicion[1],
            satelite->posicion[2]
        ));

        // rotación del satelite sobre sí mismo
        model = glm::rotate(
            model,
            satelite->anguloRotacion,
            glm::vec3(0.0f,1.0f,0.0f)
        );

        // Escalamos la esfera base según el tamaño del satelite.
        // La esfera original tiene tamaño 1 y aquí la multiplicamos por satelite->tamanho.
        model = glm::scale(model, glm::vec3(satelite->tamanho));

        // enviar matriz al shader
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // enviamos el color del planeta
        glUniform3fv(colorLoc, 1, glm::value_ptr(satelite->color));

        // dibujar el satelite con los 1080 vértices de la esfera
        glBindVertexArray(satelite->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 1080);
    }
}

void actualizarMovimientoSat(std::vector<Satelite*>& satelites, Planeta* tierra)
{
    for (Satelite* satelite : satelites)
    {
        satelite->anguloTraslacion += satelite->velocidadTraslacion;
        satelite->anguloRotacion += satelite->velocidadRotacion;

        float ang = satelite->anguloTraslacion;

        satelite->posicion[0] = tierra->posicion[0] + satelite->distanciaTierra * cos(ang);
        satelite->posicion[2] = tierra->posicion[2] + satelite->distanciaTierra * sin(ang);
    }
}