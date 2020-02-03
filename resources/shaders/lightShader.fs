#version 330 core
in vec3 fNormal;
in vec3 FragPos;
out vec4 FragColor;

uniform vec3 camPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

void main(){

    //ambient lighting
    
    vec3 ambient = light.ambient * material.ambient;

    //diffused lighting
    vec3 norm = normalize(fNormal);
    vec3 lightDir = normalize(FragPos - light.position);
    float diff =  max(dot(norm, lightDir), 0);
    vec3 diffused = (diff * material.diffuse) * light.diffuse;

    //specular lighting
    vec3 viewDir = - normalize(FragPos - camPos);
    vec3 reflectDir = reflect(lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (spec * material.specular) * light.specular;

    vec3 result = ambient + diffused + specular;
    FragColor = vec4(result, 1.0);
}