layout (location = 0) in vec3 aPos;
layout (location = 3) in vec3 aNormal;

uniform mat4 model;
uniform mat3 normalMatrix;
uniform mat4 projection;
uniform mat4 view;

out vec4 vertexColor; // specify a color output to the fragment shader
out vec3 normal;
out vec3 fragPos;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	fragPos = vec3(model * vec4(aPos, 1.0));
	vertexColor = vec4(1.0, 0.0, 0.0, 1.0); // output variable to red
	normal = normalMatrix * aNormal;
};