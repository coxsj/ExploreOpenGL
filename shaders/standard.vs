#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

uniform mat4 model;
uniform mat3 normalMatrix;
uniform mat4 projection;
uniform mat4 view;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoord;
out vec4 vertexColor; // specify a color output to the fragment shader

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	normal = normalMatrix * aNormal;
	fragPos = vec3(model * vec4(aPos, 1.0));
	vertexColor = vec4(aColor, 1.0);
	texCoord = aTexCoord;
};