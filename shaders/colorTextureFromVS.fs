#version 330 core
in vec4 vertexColor; // input variable from vertex shader (same name and type)
in vec2 texCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

out vec4 FragColor;

void main()
{
   FragColor = mix(texture(texture0, texCoord), texture(texture1, texCoord), 0.2);
   //FragColor = texture(texture0, texCoord) * vertexColor;
};