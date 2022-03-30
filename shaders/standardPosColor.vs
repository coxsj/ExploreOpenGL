#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec4 vertexColor; // specify a color output to the fragment shader

uniform float offset; // we set this variable in our render loop

void main()
{
   gl_Position = vec4(aPos.x+offset, aPos.y, aPos.z, 1.0);
	vertexColor = vec4(aColor, 1.0);
};