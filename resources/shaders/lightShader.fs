#version 330 core
in vec3 fNormal;
in vec3 FragPos;
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main(){


    float ambientStrength = 0.1;
    vec3 ambientLight =  ambientStrength * lightColor;
    vec3 ambient = ambientLight * objectColor;

    vec3 norm = normalize(fNormal);
    vec3 lightDir = normalize(FragPos - lightPos);
    float diff =  max(dot(norm, lightDir), 0);
    vec3 diffused = diff * lightColor;

    vec3 result = (ambient + diffused) * objectColor;
    FragColor = vec4(result, 0);
}