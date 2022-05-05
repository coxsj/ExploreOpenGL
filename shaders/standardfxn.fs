#version 330 core
// Constant (enum) values
const int e_VertexColor = 0;
const int e_ObjectColor = 1;
const int e_TextureColor = 2;
const int e_LightMapColor = 3;

in vec4 vertexColor;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

struct DirLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
struct PointLight {
	vec3 position;
	float constant;
	float linear;
	float quadratic;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
struct Material {
	int colorType;
	vec4 objectColor;
	sampler2D diffuse;
	sampler2D specular;
	sampler2D tA;
	sampler2D tB;
	float shininess;
};

// Uniforms
uniform DirLight dirLight;
uniform Material material;
uniform float opacity;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform vec3 viewPos;

out vec4 FragColor;

// Function Prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

// Global variables
vec3 objectAmbientDiffuseColor;
vec3 objectSpecularColor;
	
void main()
{
	//Lighting
	//Base object color
	switch(material.colorType){
		case e_VertexColor:
			objectAmbientDiffuseColor = vec3(vertexColor.x, vertexColor.y, vertexColor.z);
			objectSpecularColor = objectAmbientDiffuseColor;
			break;
		case e_ObjectColor:
			objectAmbientDiffuseColor = vec3(material.objectColor.x, material.objectColor.y, material.objectColor.z);
			objectSpecularColor = vec3(material.objectColor.x, material.objectColor.y, material.objectColor.z);
			break;
		case e_TextureColor:
			vec4 textColor = mix(texture(material.tA, texCoord), texture(material.tB, vec2(texCoord.x, texCoord.y)), 0.2);
			objectAmbientDiffuseColor = vec3(textColor.x, textColor.y, textColor.z);
			objectSpecularColor = objectAmbientDiffuseColor;
			break;
		case e_LightMapColor:
			objectAmbientDiffuseColor = vec3(texture(material.diffuse, texCoord));
			objectSpecularColor = vec3(texture(material.specular, texCoord));
			break;
		default:
		break;
	}
	// properties
	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(viewPos - fragPos);
	// phase 1: Directional lighting
	vec3 result = CalcDirLight(dirLight, norm, viewDir);
	// phase 2: Point lights
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
		result += CalcPointLight(pointLights[i], norm, fragPos, viewDir);
	// phase 3: Spot light
	//result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
	FragColor = vec4(result, opacity); 
};

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// combine results
	vec3 ambient = light.ambient * objectAmbientDiffuseColor;
	vec3 diffuse = light.diffuse * diff * objectAmbientDiffuseColor;
	vec3 specular = light.specular * spec * objectSpecularColor;
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
	vec3 lightDir = normalize(light.position - fragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
	light.quadratic * (distance * distance));
	// combine results
	vec3 ambient = light.ambient * objectAmbientDiffuseColor;
	vec3 diffuse = light.diffuse * diff * objectAmbientDiffuseColor;
	vec3 specular = light.specular * spec * objectSpecularColor;
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}