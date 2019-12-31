// FRAGMENT SHADER
#version 330 core

out vec4 FragColor;

in vec3 fColor;
in vec2 fTexCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    FragColor = mix(texture(texture0, fTexCoord), texture(texture1, fTexCoord), 0.2) * vec4(fColor, 1.0);
}