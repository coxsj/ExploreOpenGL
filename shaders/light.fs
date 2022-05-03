#version 330 core
in vec3 normal;

uniform vec3 lightColor;
uniform float opacity;

out vec4 FragColor;

void main()
{
	FragColor = vec4(lightColor, opacity);
}