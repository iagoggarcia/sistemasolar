#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

uniform int esSol;
uniform int luzEncendida;
uniform int usarTextura;
uniform int usarMultitextura;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    vec3 colorBase;

    if (usarTextura == 1) {
        vec4 tex1 = texture(texture1, TexCoord);

        if (usarMultitextura == 1) {
            vec4 tex2 = texture(texture2, TexCoord);
            colorBase = mix(tex1, tex2, 0.35).rgb;
        } else {
            colorBase = tex1.rgb;
        }
    } else {
        colorBase = objectColor;
    }

    if (esSol == 1) {
        if (luzEncendida == 1)
            FragColor = vec4(colorBase, 1.0);
        else
            FragColor = vec4(colorBase * 0.15, 1.0);
        return;
    }

    if (luzEncendida == 0) {
        FragColor = vec4(colorBase * 0.15, 1.0);
        return;
    }

    float ambientStrength = 0.20;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 resultado = (ambient + diffuse) * colorBase;
    FragColor = vec4(resultado, 1.0);
}