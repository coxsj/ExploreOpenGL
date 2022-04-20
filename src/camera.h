#pragma once

#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


class Camera
{
private:
	//Camera variables
	glm::vec3 cameraPos{ glm::vec3(0.0f, 0.0f, 10.0f) };
	glm::vec3 cameraFront{ glm::vec3(0.0f, 0.0f, -1.0f )};
	glm::vec3 cameraUp { glm::vec3(0.0f, 1.0f, 0.0f) };

	float cameraSpeed{ 0 }; // adjust via delta time

	float zoom{ 30.0f };

	const float sensitivity = 0.01f;
	float pitch{ 0 };
	float yaw{ -90.0f };

public:
	Camera() {};
	~Camera() {};

	void keyInput(int GLFW_key, float deltaTime);
	glm::mat4 lookAt() { 
		//lookAt takes a position, target and up vector as parameters
		return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); }
	void newZoom(float yoffset);
	glm::mat4 perspective(float width, float height, float front, float back) {
		return glm::perspective(glm::radians(zoom), width / height, front, back);}
	void updateDirectionFromMouse(float xoffset, float yoffset);
};

