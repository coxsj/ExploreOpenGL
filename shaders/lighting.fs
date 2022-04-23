#version 330 core
in vec3 fragPos;
in vec3 normal;

uniform float ambientStrength;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 lightColor;

out vec4 FragColor;

void main()
{
	//Lighting
	//Ambient
	vec3 ambient = ambientStrength * lightColor;
	//Diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	vec3 result = (ambient + diffuse) * objectColor;
	FragColor = vec4(result, 1.0);
}
