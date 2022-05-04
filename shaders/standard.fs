#version 330 core
in vec4 vertexColor;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

struct Material {
	bool useLightMaps;
	bool useTextures;
	bool useVertexColor;
	vec4 objectColor;
	sampler2D diffuse;
	sampler2D specular;
	sampler2D tA;
	sampler2D tB;
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
	//Base object color
	vec3 objectAmbientDiffuseColor = vec3(material.objectColor.x, material.objectColor.y, material.objectColor.z);
	vec3 objectSpecularColor = vec3(material.objectColor.x, material.objectColor.y, material.objectColor.z);
	if(material.useVertexColor){
		objectAmbientDiffuseColor = vec3(vertexColor.x, vertexColor.y, vertexColor.z);
		objectSpecularColor = objectAmbientDiffuseColor;
	}
	if(material.useTextures){
		vec4 textColor = mix(texture(material.tA, texCoord), texture(material.tB, vec2(texCoord.x, texCoord.y)), 0.2);
		objectAmbientDiffuseColor = vec3(textColor.x, textColor.y, textColor.z);
		objectSpecularColor = objectAmbientDiffuseColor;
	}
	if(material.useLightMaps) {
		objectAmbientDiffuseColor = vec3(texture(material.diffuse, texCoord));
		objectSpecularColor = vec3(texture(material.specular, texCoord));
	}
	//Ambient
	vec3 ambient = light.ambient * objectAmbientDiffuseColor;
	//Diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * objectAmbientDiffuseColor;
	//Specular
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * objectSpecularColor;
	//Combined Phong lighting effect
	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, opacity); 
};