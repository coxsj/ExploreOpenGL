#include "camera.h"
#include "glfw-3.3.6\include\GLFW\glfw3.h"
#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

void Camera::keyInput(int GLFW_key, float deltaTime) {
	cameraSpeed = 2.5f * deltaTime;

	if(GLFW_key == GLFW_KEY_W)	cameraPos += cameraSpeed * cameraFront;
	if (GLFW_key == GLFW_KEY_S)	cameraPos -= cameraSpeed * cameraFront;
	if (GLFW_key == GLFW_KEY_A) 
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (GLFW_key == GLFW_KEY_D) 
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	//FPV camera controls
	if (GLFW_key == GLFW_KEY_UP)	cameraPos += cameraSpeed * cameraFront;
	if (GLFW_key == GLFW_KEY_DOWN)	cameraPos -= cameraSpeed * cameraFront;
	if (GLFW_key == GLFW_KEY_LEFT)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (GLFW_key == GLFW_KEY_RIGHT)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	// make sure the user stays at the ground level
	cameraPos.y = 0.0f; // <-- this one-liner keeps the user at the ground level (xz plane)
}
void Camera::newZoom(float yoffset) {
	zoom -= yoffset;
	if (zoom < 1.0f) zoom = 1.0f;
	else if (zoom > 150.0f) zoom = 150.0f;
}
void Camera::updateDirectionFromMouse(float xoffset, float yoffset) {
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	//Update pitch and yaw based on mouse changes
	yaw += xoffset;
	pitch += yoffset;

	//Aply limits to prevent camera flip
	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	//Calculate the new camera direction vector
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	//Update the global camera vector
	cameraFront = glm::normalize(direction);
}