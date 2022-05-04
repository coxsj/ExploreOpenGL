#pragma once
#include <string>
#include "GLFW\glfw3.h"

//Structs
struct TextureFileData {
	std::string filename;
	bool		flipImage;
	GLenum		format;
};

// OpenGL enum values
const int VERTEX_COLOR = 0;
const int OBJECT_COLOR = 1;
const int TEXTURE_COLOR = 2;
const int LIGHTMAP_COLOR = 3;

// Screen Settings
constexpr unsigned int SCR_WIDTH = 1800;
constexpr unsigned int SCR_HEIGHT = 1600;

// Resource files
const std::string SHADER_DIR("C:\\Users\\coxst\\OneDrive\\Documents\\Repos\\OpenGL\\ExploreOpenGL\\shaders\\");
const std::string RESOURCE_DIR("C:\\Users\\coxst\\OneDrive\\Documents\\Repos\\OpenGL\\ExploreOpenGL\\resources\\");

//Magic numbers
constexpr unsigned int	VERTICES_PER_TRIANGLE = 3;
const std::string		UNNAMED{ "UNNAMED" };