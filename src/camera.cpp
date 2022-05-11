#include "camera.h"
#include "glfw-3.3.6\include\GLFW\glfw3.h"
#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

void Camera::keyInput(int GLFW_key, float deltaTime) {
	float velocity = cameraSpeed * deltaTime;
	if(GLFW_key == GLFW_KEY_W)	cameraPos += cameraFront * velocity;
	if (GLFW_key == GLFW_KEY_S)	cameraPos -= cameraFront * velocity;
	if (GLFW_key == GLFW_KEY_A) 
		cameraPos -= cameraRight * velocity;
	if (GLFW_key == GLFW_KEY_D) 
		cameraPos += cameraRight * velocity;
}
void Camera::newZoom(float yoffset) {
	zoom -= yoffset;
	if (zoom < 1.0f) zoom = 1.0f;
	else if (zoom > 45.0f) zoom = 45.0f;
}
void Camera::updateCameraVectors() {
	// calculates the front vector from the Camera's (updated) Euler Angles
	// calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
	// also re-calculate the Right and Up vector
	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}
void Camera::updateDirectionFromMouse(float xoffset, float yoffset, bool constrainPitch) {
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	//Update pitch and yaw based on mouse changes
	yaw += xoffset;
	pitch += yoffset;

	if (constrainPitch) {
		//Apply limits to prevent camera flip
		if (pitch > 89.0f) pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;
	}
	updateCameraVectors();
}
