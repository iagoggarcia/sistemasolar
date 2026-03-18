#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;

// esta es la matriz de la camara, o sea, desde donde estamos mirando
uniform mat4 view;

// esta aplica la perspectiva para que la escena 3D se vea bien en pantalla
uniform mat4 projection;

void main()
{
    // primero colocamos el planeta con model,
    // luego aplicamos la camara con view
    // y al final la perspectiva con projection
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}