layout (location = 0) in vec3 aPos;
layout (location = 3) in vec3 aNormal;

out vec4 vertexColor; // specify a color output to the fragment shader
out vec3 normal;
out vec3 fragPos;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	fragPos = vec3(model * vec4(aPos, 1.0));
	vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // output variable to dark-red
	normal = aNormal;
};