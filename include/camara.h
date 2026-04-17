#ifndef CAMARA_H
#define CAMARA_H

#include <glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "cuerpo.h"

// objetivos actuales de la cámara
extern CuerpoCeleste* cuerpoObjetivo;

// parámetros de la cámara al enfocar un objetivo
extern float distanciaCamara;
extern glm::vec3 direccionCamara;
extern bool vistaDesdeLuna;
extern bool vistaDesdeTierra;

// estado de la cámara general libre
extern glm::vec3 posicionCamaraGeneral;
extern glm::vec3 direccionCamaraGeneral;
extern glm::vec3 upCamaraGeneral;

// estado global de la escena controlado desde la entrada.
extern bool mostrarOrbitas;
extern bool teclaOPulsadaAntes;
extern float factorVelocidad;

void enfocarMarte(std::vector<CuerpoCeleste*>& cuerpos);
void enfocarTierraDesdeLuna(std::vector<CuerpoCeleste*>& cuerpos);
void enfocarIssDesdeTierra(std::vector<CuerpoCeleste*>& cuerpos);
void ponerCamaraGeneral();
void enfocarPlanetaDesdeTierra(CuerpoCeleste* cuerpo);
void enfocarSateliteDesdeTierra(CuerpoCeleste* cuerpo);

void avanzarCamaraGeneral(float deltaTime);
void retrocederCamaraGeneral(float deltaTime);
void girarCamaraGeneralIzquierda(float deltaTime);
void girarCamaraGeneralDerecha(float deltaTime);

glm::vec3 obtenerTargetActual();
glm::vec3 obtenerPosicionCamaraActual(std::vector<CuerpoCeleste*>& cuerpos, glm::vec3 target);
glm::mat4 obtenerVista(std::vector<CuerpoCeleste*>& cuerpos);

#endif