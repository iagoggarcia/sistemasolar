#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform bool esSol;
uniform bool luzEncendida;

void main()
{
    if (esSol) {
        if (luzEncendida) {
            vec3 colorSol = 1.2 * objectColor;
            FragColor = vec4(colorSol, 1.0);
        } else {
            FragColor = vec4(0.2 * objectColor, 1.0);
        }
        return;
    }

    float ambientI = 0.15;
    vec3 ambient = ambientI * lightColor;

    vec3 result = ambient * objectColor;

    if (luzEncendida) {
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        result += diffuse * objectColor;
    }

    FragColor = vec4(result, 1.0);
}