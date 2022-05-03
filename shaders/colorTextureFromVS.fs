#version 330 core
in vec4 vertexColor; // input variable from vertex shader (same name and type)
in vec2 texCoord;
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
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
	//Object color
	vec4 objectColor = mix(texture(texture0, texCoord), texture(texture1, vec2(texCoord.x, texCoord.y)), 0.2);
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
	vec3 result = (ambient + diffuse + specular) * vec3(objectColor.x, objectColor.y, objectColor.z);
	FragColor = vec4(result, opacity); 
};