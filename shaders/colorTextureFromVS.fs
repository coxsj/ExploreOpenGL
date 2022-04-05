#version 330 core
in vec4 vertexColor; // input variable from vertex shader (same name and type)
in vec2 texCoord;

uniform sampler2D ourTexture;
uniform sampler2D texture2;

out vec4 FragColor;

void main()
{
   FragColor = mix(texture(ourTexture, texCoord), texture(texture2, texCoord), 0.2);
   //FragColor = texture(ourTexture, texCoord) * vertexColor;
};