#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in vec3 aNormal;

uniform mat3 normalMatrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec3 fragPos;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	fragPos = vec3(model * vec4(aPos, 1.0));
	vec4 adsjustedNormal = projection * view * model * vec4(aNormal, 1.0);
	normal = normalMatrix * aNormal;
	//normal = vec3(adsjustedNormal.x, adsjustedNormal.y, adsjustedNormal.z);
}