#pragma once

#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace camDefault {
	const glm::vec3	cameraFront{ glm::vec3(0.0f, 0.0f, -1.0f) };
	const glm::vec3	cameraPos{ glm::vec3(0.0f, 0.0f, 10.0f) };
	const float		cameraSpeed{ 2.5f };
	const glm::vec3	cameraUp{ glm::vec3(0.0f, 1.0f, 0.0f) };
	const float		pitch{ 0 };
	const float		mouseSensitivity{ 0.01f };
	const glm::vec3	worldUp{ glm::vec3(0.0f, 1.0f, 0.0f) };
	const float		yaw{ -90.0f };
	const float		zoom{ 45.0f };
}

class Camera
{
private:
	//Camera variables
	glm::vec3	cameraFront;
	glm::vec3	cameraPos;
	glm::vec3	cameraRight;
	float		cameraSpeed; // adjust via delta time
	glm::vec3	cameraUp;
	float		pitch;
	float		mouseSensitivity;
	glm::vec3	worldUp;
	float		yaw;
	float		zoom;
	
public:
	Camera():
		cameraFront(camDefault::cameraFront),
		cameraPos(camDefault::cameraPos),
		cameraSpeed(camDefault::cameraSpeed),
		cameraUp(camDefault::cameraUp),
		pitch(camDefault::pitch),
		mouseSensitivity(camDefault::mouseSensitivity),
		worldUp(camDefault::worldUp),
		yaw(camDefault::yaw),
		zoom(camDefault::zoom) {
		updateCameraVectors();
	};
	// constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float y, float p) : 
		cameraPos (glm::vec3(posX, posY, posZ)), cameraUp (glm::vec3(upX, upY, upZ)),
		cameraFront(camDefault::cameraFront), cameraSpeed(camDefault::cameraSpeed),
		mouseSensitivity(camDefault::mouseSensitivity), zoom(camDefault::zoom), 
		worldUp(camDefault::worldUp), yaw (y), pitch (p){
		updateCameraVectors();
	}
	~Camera() {};

	void keyInput(int GLFW_key, float deltaTime);
	glm::mat4 lookAt() { 
		//lookAt takes a position, target and up vector as parameters
		return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); }
	void newZoom(float yoffset);
	glm::mat4 perspective(float width, float height, float front, float back) {
		return glm::perspective(glm::radians(zoom), width / height, front, back);}
	glm::vec3 getPos() {return cameraPos; }
	void updateCameraVectors();
	void updateDirectionFromMouse(float xoffset, float yoffset, bool constrainPitch = false);
};

