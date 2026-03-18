#include <stdio.h>
#include <stdlib.h>
#include <planetas.h>
#include <string.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

Planeta *crearPlaneta(const char *nombre, float tamanho, float py, float pz, float distancia, float velTras, float velRot, glm::vec3 color, GLuint VAO)
{

    Planeta *planeta = (Planeta *)malloc(sizeof(Planeta));

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
    planeta->posicion[0] = distancia;
    planeta->posicion[1] = py;
    planeta->posicion[2] = pz;

    if (distancia > 0.0f) {
        planeta->VAOorbita = crearVAOorbita(distancia);
        planeta->numVerticesOrbita = 100;
    }
    else { // si es el sol no va a tener órbita
        planeta->VAOorbita = 0;
        planeta->numVerticesOrbita = 0;
    }

    return planeta;
}

std::vector<Planeta *> inicializarPlanetas(GLuint VAO_esfera)
{
    std::vector<Planeta *> planetas;
    planetas.push_back(crearPlaneta("Sol",      0.50f, 0.0f, 0.0f, 0.0f,  0.0f,   0.015f, glm::vec3(1.0f, 0.8f, 0.0f), VAO_esfera));

    planetas.push_back(crearPlaneta("Mercurio", 0.05f, 0.0f, 0.0f, 1.2f,  0.028f, 0.015f, glm::vec3(0.6f, 0.6f, 0.6f), VAO_esfera));
    planetas.push_back(crearPlaneta("Venus",    0.08f, 0.0f, 0.0f, 1.8f,  0.022f, 0.015f, glm::vec3(0.9f, 0.7f, 0.2f), VAO_esfera));
    planetas.push_back(crearPlaneta("Tierra",   0.09f, 0.0f, 0.0f, 2.4f,  0.018f, 0.022f, glm::vec3(0.2f, 0.4f, 1.0f), VAO_esfera));
    planetas.push_back(crearPlaneta("Marte",    0.07f, 0.0f, 0.0f, 3.0f,  0.014f, 0.022f, glm::vec3(0.8f, 0.3f, 0.1f), VAO_esfera));

    planetas.push_back(crearPlaneta("Júpiter",  0.20f, 0.0f, 0.0f, 3.8f,  0.010f, 0.028f, glm::vec3(0.8f, 0.6f, 0.4f), VAO_esfera));
    planetas.push_back(crearPlaneta("Saturno",  0.17f, 0.0f, 0.0f, 4.7f,  0.008f, 0.028f, glm::vec3(0.9f, 0.8f, 0.5f), VAO_esfera));
    planetas.push_back(crearPlaneta("Urano",    0.14f, 0.0f, 0.0f, 5.5f,  0.006f, 0.028f, glm::vec3(0.5f, 0.8f, 1.0f), VAO_esfera));
    planetas.push_back(crearPlaneta("Neptuno",  0.14f, 0.0f, 0.0f, 6.3f,  0.0045f,0.028f, glm::vec3(0.2f, 0.3f, 0.9f), VAO_esfera));

    return planetas;
}

void dibujarPlanetas(std::vector<Planeta *> &planetas, GLuint modelLoc, GLuint colorLoc)
{
    for (Planeta *planeta : planetas)
    {
        // matriz modelo
        glm::mat4 model = glm::mat4(1.0f);

        // mover el planeta a su distancia del sol (por ahora 'y' y 'z' los dejamos como están q no tengo ni idea de los valores)
        model = glm::translate(model, glm::vec3(
            planeta->posicion[0],
            planeta->posicion[1],
            planeta->posicion[2]
        ));

        // rotación del planeta sobre sí mismo
        model = glm::rotate(
            model,
            planeta->anguloRotacion,
            glm::vec3(0.0f,1.0f,0.0f)
        );

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

void actualizarMovimiento(std::vector<Planeta*>& planetas)
{
    float tiempo = glfwGetTime();

    for (Planeta* planeta : planetas)
    {
        // actualizamos el ángulo de traslación según su velocidad
        planeta->anguloTraslacion += planeta->velocidadTraslacion;

        // actualizamos el ángulo de rotación sobre su propio eje
        planeta->anguloRotacion += planeta->velocidadRotacion;

        float ang = planeta->anguloTraslacion;

        // calculamos la posición en la órbita circular alrededor del sol
        planeta->posicion[0] = planeta->distanciaAlSol * cos(ang);
        planeta->posicion[2] = planeta->distanciaAlSol * sin(ang);
    }
}

std::vector<float> crearVerticesOrbita(float radio) {
    std::vector<float> vertices;
    int segmentos = 100;

    for (int i = 0; i < segmentos; i++) {
        float angulo = 2.0f * M_PI * i / segmentos;

        float x = radio * cos(angulo);
        float y = 0.0f;
        float z = radio * sin(angulo);

        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
    }

    return vertices;
}


GLuint crearVAOorbita(float radio) {
    std::vector<float> vertices = crearVerticesOrbita(radio);
    int segmentos = 100;
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}


void dibujarOrbitas(std::vector<Planeta*>& planetas, GLuint modelLoc, GLuint colorLoc) {
    for (Planeta* planeta : planetas) {
        if (planeta->distanciaAlSol <= 0.0f) continue; // saltamos el sol porque no se traslada

        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glm::vec3 colorOrbita(0.7f, 0.7f, 0.7f);
        glUniform3fv(colorLoc, 1, glm::value_ptr(colorOrbita));

        glBindVertexArray(planeta->VAOorbita);
        glDrawArrays(GL_LINE_LOOP, 0, planeta->numVerticesOrbita);
    }
}