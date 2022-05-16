#version 330 core
in vec2 texCoords;
in vec4 vertexColor; 

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

out vec4 fragColor;

void main()
{    
    fragColor = texture(texture_diffuse1, texCoords);
    //fragColor = vertexColor;
}