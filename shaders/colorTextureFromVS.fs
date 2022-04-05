#version 330 core
out vec4 FragColor;
in vec4 vertexColor; // input variable from vertex shader (same name and type)
in vec2 texCoord;

uniform sampler2D ourTexture;

void main()
{
   FragColor = texture(ourTexture, texCoord) * vertexColor;
};