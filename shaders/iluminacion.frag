#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform int usarTextura;
uniform int usarMultitextura;
uniform int esSol;
uniform int luzEncendida;

void main()
{
    vec3 baseColor = objectColor;

    if (usarTextura == 1) {
        baseColor = texture(texture1, TexCoord).rgb;

        if (usarMultitextura == 1) {
            vec3 capa2 = texture(texture2, TexCoord).rgb;
            baseColor = mix(baseColor, capa2, 0.35);
        }
    }

    // si la luz está apagada, todo queda apagado, incluido el Sol
    if (luzEncendida == 0) {
        vec3 luzMinima = baseColor * 0.4;
        FragColor = vec4(luzMinima, 1.0);
        return;
    }

    // solo si la luz está encendida, el Sol emite luz
    if (esSol == 1) {
        FragColor = vec4(baseColor, 1.0);
        return;
    }

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    // ambiente
    float ambientI = 0.12;
    vec3 ambient = ambientI * lightColor;

    // difusa
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // especular
    float specularStrength = 0.6;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * baseColor;
    FragColor = vec4(result, 1.0);
}