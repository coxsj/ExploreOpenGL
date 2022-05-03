#pragma once
#include <string>
#include "GLFW\glfw3.h"

//Structs
struct TextureFileData {
	std::string filename;
	bool		flipImage;
	GLenum		format;
};
// Settings
constexpr unsigned int SCR_WIDTH = 1800;
constexpr unsigned int SCR_HEIGHT = 1600;

const std::string shaderDir("C:\\Users\\coxst\\OneDrive\\Documents\\Repos\\OpenGL\\ExploreOpenGL\\shaders\\");
const std::string RESOURCE_DIR("C:\\Users\\coxst\\OneDrive\\Documents\\Repos\\OpenGL\\ExploreOpenGL\\resources\\");

//Magic numbers
constexpr unsigned int	VERTICES_PER_TRIANGLE = 3;
const std::string		UNNAMED{ "UNNAMED" };