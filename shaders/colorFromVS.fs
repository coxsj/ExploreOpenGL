#version 330 core
out vec4 FragColor;
in vec4 vertexColor; // input variable from vertex shader (same name and type)
in vec3 vertexPosition;
void main()
{
   FragColor = vec4(vertexPosition.x, vertexPosition.y, vertexPosition.z, 1.0);
};