#version 330 core
in vec3 normal;

uniform vec3 lightSourceColor;
uniform float opacity;

out vec4 FragColor;

void main()
{
	FragColor = vec4(lightSourceColor, opacity);
}