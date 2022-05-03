#version 330 core
in vec4 vertexColor; // input variable from vertex shader (same name and type)
in vec3 normal;
in vec3 fragPos;

struct Material {
vec3 ambient;
vec3 diffuse;
vec3 specular;
float shininess;
};
struct Light {
vec3 position;
vec3 ambient;
vec3 diffuse;
vec3 specular;
};
uniform Light light;
uniform Material material;
uniform float opacity;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
	//Lighting
	//Ambient
	vec3 ambient = light.ambient * material.ambient;
	//Diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);
	//Specular
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);
	//Combined Phong lighting effect
	vec3 result = (ambient + diffuse + specular) * vec3(vertexColor.x, vertexColor.y, vertexColor.z);
	FragColor = vec4(result, opacity);
};