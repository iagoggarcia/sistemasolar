#include "cuerpo.h"
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

GLuint cargarTextura(const char* ruta) {
    GLuint textura;
    glGenTextures(1, &textura);
    glBindTexture(GL_TEXTURE_2D, textura);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(ruta, &width, &height, &nrChannels, 0);

    if (data) {
        GLenum formato = GL_RGB;
        if (nrChannels == 1) formato = GL_RED;
        else if (nrChannels == 3) formato = GL_RGB;
        else if (nrChannels == 4) formato = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, formato, width, height, 0, formato, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Error cargando textura: " << ruta << std::endl;
    }

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return textura;
}

CuerpoCeleste* crearCuerpo(const char* nombre, float tamanho, float velTras, float velRot, float distancia, float inclinacionOrbita, glm::vec3 color, GLuint VAO, CuerpoCeleste* padre) {
    CuerpoCeleste* cuerpo = (CuerpoCeleste*)malloc(sizeof(CuerpoCeleste));

    strcpy(cuerpo->nombre, nombre);

    cuerpo->tamanho = tamanho;
    cuerpo->distanciaAlPadre = distancia;

    cuerpo->velocidadTraslacion = velTras;
    cuerpo->velocidadRotacion = velRot;

    cuerpo->anguloTraslacion = 0.0f;
    cuerpo->anguloRotacion = 0.0f;

    cuerpo->inclinacionOrbita = inclinacionOrbita;

    cuerpo->color = color;
    cuerpo->VAO = VAO;
    cuerpo->padre = padre;

    cuerpo->textura1 = 0;
    cuerpo->textura2 = 0;
    cuerpo->multitextura = false;

    // Posición inicial: si no tiene padre orbita el origen; si tiene, parte desde la posición del padre
    if (padre == nullptr) {
        cuerpo->posicion[0] = distancia;
        cuerpo->posicion[1] = 0.0f;
        cuerpo->posicion[2] = 0.0f;
    } else {
        cuerpo->posicion[0] = padre->posicion[0] + distancia;
        cuerpo->posicion[1] = padre->posicion[1];
        cuerpo->posicion[2] = padre->posicion[2];
    }

    if (distancia > 0.0f) {
        cuerpo->VAOorbita = crearVAOorbita(distancia);
        cuerpo->numVerticesOrbita = 100;
    }
    else { // si es el sol no va a tener órbita
        cuerpo->VAOorbita = 0;
        cuerpo->numVerticesOrbita = 0;
    }

    return cuerpo;
}

void actualizarMovimiento(std::vector<CuerpoCeleste*>& cuerpos, float deltaTime, float factorVelocidad) {
    for (CuerpoCeleste* c : cuerpos) {
        c->anguloTraslacion += c->velocidadTraslacion * deltaTime * factorVelocidad;
        c->anguloRotacion   += c->velocidadRotacion   * deltaTime * factorVelocidad;

        float ang = c->anguloTraslacion;

        if (c->padre == nullptr) {
            float x = c->distanciaAlPadre * cos(ang);
            float zBase = c->distanciaAlPadre * sin(ang);

            c->posicion[0] = x;
            c->posicion[1] = -zBase * sin(c->inclinacionOrbita);
            c->posicion[2] = zBase * cos(c->inclinacionOrbita);
        } else {
            float x = c->distanciaAlPadre * cos(ang);
            float zBase = c->distanciaAlPadre * sin(ang);

            c->posicion[0] = c->padre->posicion[0] + x;
            c->posicion[1] = c->padre->posicion[1] - zBase * sin(c->inclinacionOrbita);
            c->posicion[2] = c->padre->posicion[2] + zBase * cos(c->inclinacionOrbita);
        }
    }
}

std::vector<CuerpoCeleste*> inicializarCuerpos(GLuint VAO_esfera) {
    std::vector<CuerpoCeleste*> cuerpos;

    // planetas (padre = nullptr)
    CuerpoCeleste* sol = crearCuerpo("Sol",      0.50f, 0.0f,  0.9f, 0.0f,  0.0f, glm::vec3(1.0f, 0.8f, 0.0f), VAO_esfera, nullptr);
    CuerpoCeleste* mercurio = crearCuerpo("Mercurio", 0.05f, 1.8f,  0.9f, 1.2f,  0.0f, glm::vec3(0.6f, 0.6f, 0.6f), VAO_esfera, nullptr);
    CuerpoCeleste* venus = crearCuerpo("Venus",    0.08f, 1.4f,  0.9f, 1.8f,  0.0f, glm::vec3(0.9f, 0.7f, 0.2f), VAO_esfera, nullptr);
    CuerpoCeleste* tierra = crearCuerpo("Tierra",   0.09f, 1.1f,  1.3f, 2.4f,  0.0f, glm::vec3(0.2f, 0.4f, 1.0f), VAO_esfera, nullptr);
    CuerpoCeleste* marte = crearCuerpo("Marte",    0.07f, 0.9f,  1.3f, 3.0f,  0.0f, glm::vec3(0.8f, 0.3f, 0.1f), VAO_esfera, nullptr);
    CuerpoCeleste* jupiter = crearCuerpo("Júpiter",  0.20f, 0.6f,  1.6f, 3.8f,  0.0f, glm::vec3(0.8f, 0.6f, 0.4f), VAO_esfera, nullptr);
    CuerpoCeleste* saturno = crearCuerpo("Saturno",  0.17f, 0.45f, 1.6f, 4.7f,  0.0f, glm::vec3(0.9f, 0.8f, 0.5f), VAO_esfera, nullptr);
    CuerpoCeleste* urano = crearCuerpo("Urano",    0.14f, 0.32f, 1.6f, 5.5f,  0.0f, glm::vec3(0.5f, 0.8f, 1.0f), VAO_esfera, nullptr);
    CuerpoCeleste* neptuno = crearCuerpo("Neptuno",  0.14f, 0.24f, 1.6f, 6.3f,  0.0f, glm::vec3(0.2f, 0.3f, 0.9f), VAO_esfera, nullptr);

    CuerpoCeleste* luna = crearCuerpo("Luna", 0.03f, 2.2f, 0.9f, 0.22f, 0.089f, glm::vec3(0.75f, 0.75f, 0.75f), VAO_esfera, tierra);
    CuerpoCeleste* iss = crearCuerpo("ISS",  0.01f, 4.8f, 1.3f, 0.12f, 0.900f, glm::vec3(0.90f, 0.90f, 0.95f), VAO_esfera, tierra);

    sol->textura1 = cargarTextura("texturas/sol.jpg");

    mercurio->textura1 = cargarTextura("texturas/mercurio.jpg");
    venus->textura1 = cargarTextura("texturas/venus.jpg");

    tierra->textura1 = cargarTextura("texturas/tierra.jpg");
    tierra->textura2 = cargarTextura("texturas/nubes_tierra.jpg");
    tierra->multitextura = true;

    marte->textura1 = cargarTextura("texturas/marte.jpg");
    marte->textura2 = cargarTextura("texturas/marte_detalle.jpg");
    marte->multitextura = true;

    jupiter->textura1 = cargarTextura("texturas/jupiter.jpg");
    saturno->textura1 = cargarTextura("texturas/saturno.jpg");
    urano->textura1 = cargarTextura("texturas/urano.jpg");
    neptuno->textura1 = cargarTextura("texturas/neptuno.jpg");
    luna->textura1 = cargarTextura("texturas/luna.jpg");

    cuerpos.push_back(sol);
    cuerpos.push_back(mercurio);
    cuerpos.push_back(venus);
    cuerpos.push_back(tierra);
    cuerpos.push_back(marte);
    cuerpos.push_back(jupiter);
    cuerpos.push_back(saturno);
    cuerpos.push_back(urano);
    cuerpos.push_back(neptuno);
    cuerpos.push_back(luna);
    cuerpos.push_back(iss);

    return cuerpos;
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

void dibujarOrbitas(std::vector<CuerpoCeleste*>& cuerpos, GLuint modelLoc, GLuint objectColorLoc) {
    for (CuerpoCeleste* c : cuerpos) {
        if (c->distanciaAlPadre <= 0.0f) continue;

        glm::mat4 model = glm::mat4(1.0f);

        if (c->padre != nullptr) {
            model = glm::translate(model, glm::vec3(
                c->padre->posicion[0],
                c->padre->posicion[1],
                c->padre->posicion[2]
            ));
        }

        model = glm::rotate(
            model,
            c->inclinacionOrbita,
            glm::vec3(1.0f, 0.0f, 0.0f)
        );

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glm::vec3 colorOrbita(0.7f, 0.7f, 0.7f);
        glUniform3fv(objectColorLoc, 1, glm::value_ptr(colorOrbita));

        extern GLuint shaderProgram;
        GLuint esSolLoc = glGetUniformLocation(shaderProgram, "esSol");
        GLuint usarTexturaLoc = glGetUniformLocation(shaderProgram, "usarTextura");
        GLuint usarMultitexturaLoc = glGetUniformLocation(shaderProgram, "usarMultitextura");

        glUniform1i(esSolLoc, 0);
        glUniform1i(usarTexturaLoc, 0);
        glUniform1i(usarMultitexturaLoc, 0);

        glBindVertexArray(c->VAOorbita);
        glDrawArrays(GL_LINE_LOOP, 0, c->numVerticesOrbita);
    }
}

void dibujarCuerpos(std::vector<CuerpoCeleste*>& cuerpos, GLuint modelLoc, GLuint objectColorLoc, GLuint esSolLoc) {
    extern GLuint shaderProgram;
    GLuint usarTexturaLoc = glGetUniformLocation(shaderProgram, "usarTextura");
    GLuint usarMultitexturaLoc = glGetUniformLocation(shaderProgram, "usarMultitextura");

    for (CuerpoCeleste* cuerpo : cuerpos)
    {
        // matriz modelo
        glm::mat4 model = glm::mat4(1.0f);

        // mover el cuerpo a su posición actual
        model = glm::translate(model, glm::vec3(
            cuerpo->posicion[0],
            cuerpo->posicion[1],
            cuerpo->posicion[2]
        ));

        // rotación del cuerpo sobre sí mismo
        model = glm::rotate(
            model,
            cuerpo->anguloRotacion,
            glm::vec3(0.0f,1.0f,0.0f)
        );

        // Escalamos la esfera base según el tamaño del cuerpo.
        model = glm::scale(model, glm::vec3(cuerpo->tamanho));

        // enviar matriz al shader
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // enviamos el color del cuerpo
        glUniform3fv(objectColorLoc, 1, glm::value_ptr(cuerpo->color));

        if (strcmp(cuerpo->nombre, "Sol") == 0)
            glUniform1i(esSolLoc, 1);
        else
            glUniform1i(esSolLoc, 0);

        if (cuerpo->textura1 != 0) {
            glUniform1i(usarTexturaLoc, 1);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, cuerpo->textura1);

            if (cuerpo->multitextura && cuerpo->textura2 != 0) {
                glUniform1i(usarMultitexturaLoc, 1);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, cuerpo->textura2);
            } else {
                glUniform1i(usarMultitexturaLoc, 0);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
        } else {
            glUniform1i(usarTexturaLoc, 0);
            glUniform1i(usarMultitexturaLoc, 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        // dibujar el cuerpo con los 1080 vértices de la esfera
        glBindVertexArray(cuerpo->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 1080);
    }

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}