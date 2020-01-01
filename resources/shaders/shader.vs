// VERTEX SHADER
#version 330 core

layout(location=0) in vec3 vPos;
layout(location=1) in vec3 vColor;
layout(location=2) in vec2 vTexCoord;

uniform mat4 transform;

out vec3 fColor;
out vec2 fTexCoord;

void main()
{
    gl_Position = transform * vec4(vPos.x, vPos.y, vPos.z, 1.0);
    fColor = vColor;
    fTexCoord = vTexCoord;
}