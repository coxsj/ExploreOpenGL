#version 330 core
in vec4 vertexColor; // input variable from vertex shader (same name and type)
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

uniform float ambientStrength;
uniform vec3 lightPos;
uniform vec3 lightColor;
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
	vec3 ambient = ambientStrength * lightColor;
	//Diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	//Specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;
	//Commbined Phong lighting effect
	vec3 result = (ambient + diffuse + specular) * vec3(objectColor.x, objectColor.y, objectColor.z);
	//Note: Normals must be added to models in order to pick up lighting effects
	FragColor = vec4(result, 1.0); 
	
	//FragColor = mix(texture(texture0, texCoord), texture(texture1, vec2(texCoord.x, texCoord.y)), 0.2);
	//FragColor = texture(texture0, texCoord) * vertexColor;
};