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
#define M_PI 3.14159265358979323846 // definimos pi para las órbitas
#endif

GLuint cargarTextura(const char* ruta) {
    GLuint textura;
    glGenTextures(1, &textura); // guardamos un identificador para la textura
    glBindTexture(GL_TEXTURE_2D, textura); // hace que la textura pase a ser la activa

    // ajustes de la textura si se sale de la normal (vertical u horizontalmente)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // ancho de la imagen, altura y número de canales de color
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // invierte la imagen verticalmente al cargarla, si no las texturas salen al revés
    unsigned char* data = stbi_load(ruta, &width, &height, &nrChannels, 0); // lee la imagen y devuelve un puntero

    if (data) { // si el puntero es correcto
        GLenum formato = GL_RGB;
        // decide qué formato tiene según el número de canales:
        if (nrChannels == 1) formato = GL_RED;
        else if (nrChannels == 3) formato = GL_RGB;
        else if (nrChannels == 4) formato = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, formato, width, height, 0, formato, GL_UNSIGNED_BYTE, data); // copia la imagen a la textura en la GPU
        glGenerateMipmap(GL_TEXTURE_2D); // genera los mipmap de la imagen (sirven para que la textura se vea mejor cuando el cuerpo está lejos)
    } else {
        std::cout << "Error cargando textura: " << ruta << std::endl;
    }

    stbi_image_free(data); // liberamos la memoria cuando la textura ya está en al gpu
    glBindTexture(GL_TEXTURE_2D, 0); // desenlazamos la textura para que no se modifique sin querer después

    return textura;
}

CuerpoCeleste* crearCuerpo(const char* nombre, float tamanho, float velTras, float velRot, float distancia, float inclinacionOrbita, glm::vec3 color, GLuint VAO, CuerpoCeleste* padre) {
    CuerpoCeleste* cuerpo = (CuerpoCeleste*)malloc(sizeof(CuerpoCeleste));

    // guardamos todos los datos del cuerpo en su struct:
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

    // establecemos la posición inicial: si no tiene padre orbita el origen (sobre el sol); si tiene, parte desde la posición del padre
    if (padre == nullptr) {
        cuerpo->posicion[0] = distancia;
        cuerpo->posicion[1] = 0.0f;
        cuerpo->posicion[2] = 0.0f;
    } else {
        cuerpo->posicion[0] = padre->posicion[0] + distancia;
        cuerpo->posicion[1] = padre->posicion[1];
        cuerpo->posicion[2] = padre->posicion[2];
    }

    if (distancia > 0.0f) { // creamos las órbitas para todos los planetas y satélites menos el sol
        cuerpo->VAOorbita = crearVAOorbita(distancia);
        cuerpo->numVerticesOrbita = 100;
    }
    else { // si es el sol lo establecemos a 0
        cuerpo->VAOorbita = 0;
        cuerpo->numVerticesOrbita = 0;
    }

    return cuerpo;
}

void actualizarMovimiento(std::vector<CuerpoCeleste*>& cuerpos, float deltaTime, float factorVelocidad) {
    for (CuerpoCeleste* c : cuerpos) { // para todos los cuerpos
        // en los siguientes ángulos multiplicamos por deltaTime para que el movimiento sea uniforme aunque varíen los fps:
        c->anguloTraslacion += c->velocidadTraslacion * deltaTime * factorVelocidad;
        c->anguloRotacion   += c->velocidadRotacion   * deltaTime * factorVelocidad;

        float ang = c->anguloTraslacion; // guardamos aquí el valor para usarlo más cómodamente

        if (c->padre == nullptr) { // si el cuerpo no tiene padre
            // interpretamos c->distanciaAlPadre como distancia al origen (el sol)
            float x = c->distanciaAlPadre * cos(ang); // colocamos en x
            float zBase = c->distanciaAlPadre * sin(ang); // colocamos en z

            c->posicion[0] = x;
            // lo inclinamos:
            c->posicion[1] = -zBase * sin(c->inclinacionOrbita);
            c->posicion[2] = zBase * cos(c->inclinacionOrbita);
        } else {
            // en este caso el centro de la órbita será el padre
            float x = c->distanciaAlPadre * cos(ang);
            float zBase = c->distanciaAlPadre * sin(ang);

            c->posicion[0] = c->padre->posicion[0] + x;
            // lo inclinamos:
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

    // satélites (padre = tierra)
    CuerpoCeleste* luna = crearCuerpo("Luna", 0.03f, 2.2f, 0.9f, 0.22f, 0.089f, glm::vec3(0.75f, 0.75f, 0.75f), VAO_esfera, tierra);
    CuerpoCeleste* iss = crearCuerpo("ISS",  0.01f, 4.8f, 1.3f, 0.12f, 0.900f, glm::vec3(0.90f, 0.90f, 0.95f), VAO_esfera, tierra);

    // Guardamos las texturas de todos los planetas y satélites
    sol->textura1 = cargarTextura("texturas/sol.jpg");

    mercurio->textura1 = cargarTextura("texturas/mercurio.jpg");
    venus->textura1 = cargarTextura("texturas/venus.jpg");

    tierra->textura1 = cargarTextura("texturas/tierra.jpg");
    tierra->textura2 = cargarTextura("texturas/nubes_tierra.jpg"); // multitexturizado para la tierra
    tierra->multitextura = true;

    marte->textura1 = cargarTextura("texturas/marte.jpg");
    jupiter->textura1 = cargarTextura("texturas/jupiter.jpg");
    saturno->textura1 = cargarTextura("texturas/saturno.jpg");
    urano->textura1 = cargarTextura("texturas/urano.jpg");
    neptuno->textura1 = cargarTextura("texturas/neptuno.jpg");
    luna->textura1 = cargarTextura("texturas/luna.jpg");

    // ahora guardamos los cuerpos en el vector con los datos y texturas en el struct
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
    int segmentos = 100; // podría modificarse para que las órbitas queden más o menos circulares

    for (int i = 0; i < segmentos; i++) {  // para todos los segmentos
        float angulo = 2.0f * M_PI * i / segmentos; // ángulo correspondiente a ese punto de la órbita

        // calculamos los puntos de una circunferencia de radio "radio" en el plano XZ
        float x = radio * cos(angulo);
        float y = 0.0f; // la dibujamos en el plano horizontal así que y=0
        float z = radio * sin(angulo);

        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
    }

    return vertices;
}

GLuint crearVAOorbita(float radio) {
    // generamos los vértices de la órbita a partir de su radio
    std::vector<float> vertices = crearVerticesOrbita(radio);

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO); // crea el VAO que guardará la configuración de los atributos
    glGenBuffers(1, &VBO); // crea el VBO que almacenará los vértices en la GPU

    glBindVertexArray(VAO); // activamos el VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // activamos el VBO como buffer de vértices

    // copiamos los vértices de la órbita al VBO
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // indicamos cómo interpretar los datos:
    // location 0 -> 3 componentes float por vértice (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // habilitamos el atributo de posición

    // desvinculamos el VBO y el VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // devolvemos el VAO para poder dibujar la órbita después
    return VAO;
}

void dibujarOrbitas(std::vector<CuerpoCeleste*>& cuerpos) {
    extern GLuint shaderSimpleProgram;

    // obtenemos la localización de la matriz de modelo y del color en el shader simple
    GLuint modelLoc = glGetUniformLocation(shaderSimpleProgram, "model");
    GLuint colorLoc = glGetUniformLocation(shaderSimpleProgram, "color");

    for (CuerpoCeleste* c : cuerpos) {
        // si el cuerpo no tiene órbita (sol), no la dibujamos
        if (c->distanciaAlPadre <= 0.0f) continue;

        // partimos de la matriz identidad
        glm::mat4 model = glm::mat4(1.0f);

        // si el cuerpo tiene padre, trasladamos la órbita para centrarla en la posición del padre
        if (c->padre != nullptr) {
            model = glm::translate(model, glm::vec3(
                c->padre->posicion[0],
                c->padre->posicion[1],
                c->padre->posicion[2]
            ));
        }

        // aplicamos la inclinación orbital girando la órbita sobre el eje X
        model = glm::rotate(
            model,
            c->inclinacionOrbita,
            glm::vec3(1.0f, 0.0f, 0.0f)
        );

        // enviamos la matriz de modelo al shader
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // fijamos el color con el que se dibujará la órbita
        glm::vec3 colorOrbita(0.7f, 0.7f, 0.7f);
        glUniform3fv(colorLoc, 1, glm::value_ptr(colorOrbita));

        // activamos el VAO de la órbita y la dibujamos como un bucle de líneas
        glBindVertexArray(c->VAOorbita);
        glDrawArrays(GL_LINE_LOOP, 0, c->numVerticesOrbita);
    }
}

void dibujarCuerpos(std::vector<CuerpoCeleste*>& cuerpos) {
    extern GLuint shaderIluminacionProgram;

    // obtenemos las localizaciones de los uniforms necesarios en el shader de iluminación
    GLuint modelLoc             = glGetUniformLocation(shaderIluminacionProgram, "model");
    GLuint objectColorLoc       = glGetUniformLocation(shaderIluminacionProgram, "objectColor");
    GLuint esSolLoc             = glGetUniformLocation(shaderIluminacionProgram, "esSol");
    GLuint usarTexturaLoc       = glGetUniformLocation(shaderIluminacionProgram, "usarTextura");
    GLuint usarMultitexturaLoc  = glGetUniformLocation(shaderIluminacionProgram, "usarMultitextura");

    for (CuerpoCeleste* cuerpo : cuerpos)
    {
        // partimos de la matriz identidad
        glm::mat4 model = glm::mat4(1.0f);

        // trasladamos el cuerpo a su posición actual en la escena
        model = glm::translate(model, glm::vec3(
            cuerpo->posicion[0],
            cuerpo->posicion[1],
            cuerpo->posicion[2]
        ));

        // aplicamos la rotación del cuerpo sobre sí mismo
        model = glm::rotate(
            model,
            cuerpo->anguloRotacion,
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        // escalamos la esfera según el tamaño del cuerpo
        model = glm::scale(model, glm::vec3(cuerpo->tamanho));

        // enviamos la matriz de modelo y el color base al shader
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(objectColorLoc, 1, glm::value_ptr(cuerpo->color));

        // indicamos al shader si el objeto actual es el Sol
        if (strcmp(cuerpo->nombre, "Sol") == 0)
            glUniform1i(esSolLoc, 1);
        else
            glUniform1i(esSolLoc, 0);

        // si el cuerpo tiene textura, la activamos
        if (cuerpo->textura1 != 0) {
            glUniform1i(usarTexturaLoc, 1);

            // asociamos la textura principal a la unidad de textura 0
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, cuerpo->textura1);

            // si usa multitextura, activamos también la segunda textura
            if (cuerpo->multitextura && cuerpo->textura2 != 0) {
                glUniform1i(usarMultitexturaLoc, 1);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, cuerpo->textura2);
            } else {
                // si no hay segunda textura, desactivamos la multitextura
                glUniform1i(usarMultitexturaLoc, 0);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
        } else {
            // si el cuerpo no tiene texturas, indicamos al shader que no las use
            glUniform1i(usarTexturaLoc, 0);
            glUniform1i(usarMultitexturaLoc, 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        // activamos el VAO del cuerpo y dibujamos la esfera
        glBindVertexArray(cuerpo->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 1080);
    }

    // al terminar, desactivamos las texturas usadas
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}