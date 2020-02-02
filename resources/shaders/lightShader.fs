#version 330 core
in vec3 fNormal;
in vec3 FragPos;
out vec4 FragColor;

uniform vec3 camPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main(){

    //ambient lighting
    float ambientStrength = 0.1;
    vec3 ambientLight =  ambientStrength * lightColor;
    vec3 ambient = ambientLight * objectColor;

    //diffused lighting
    vec3 norm = normalize(fNormal);
    vec3 lightDir = normalize(FragPos - lightPos);
    float diff =  max(dot(norm, lightDir), 0);
    vec3 diffused = diff * lightColor;

    //specular lighting
    float specularStrength = 0.8;
    vec3 viewDir = - normalize(FragPos - camPos);
    vec3 reflectDir = reflect(lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffused + specular) * objectColor;
    FragColor = vec4(result, 0);
}