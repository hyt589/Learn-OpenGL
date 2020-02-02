// VERTEX SHADER
#version 330 core

layout(location=0) in vec3 vPos;
layout(location=1) in vec3 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fNormal;
out vec3 FragPos;

void main()
{
    gl_Position = projection * view * model * vec4(vPos.x, vPos.y, vPos.z, 1.0);
    fNormal = - mat3(transpose(inverse(model))) * vNormal;
    // fNormal = vNormal;
    FragPos = vec3(model * vec4(vPos, 1.0));
}