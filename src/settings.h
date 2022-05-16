#pragma once
#include <string>
#include "glfw-3.3.6\include\GLFW\glfw3.h"

//Structs
struct TextureFileData {
	std::string filename;
	bool		flipImage;
	GLenum		format;
};

// Fragment Shader enum values
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
const std::string TEXTURE_DIR("C:\\Users\\coxst\\OneDrive\\Documents\\Repos\\OpenGL\\ExploreOpenGL\\resources\\textures\\");
const std::string TEXTURE_DIR_GUITAR_BACKPACK("C:\\Users\\coxst\\OneDrive\\Documents\\Repos\\OpenGL\\ExploreOpenGL\\resources\\textures\\guitar_backpack\\");
const std::string TEXTURE_DIR_VIRUS_TUBE("C:\\Users\\coxst\\OneDrive\\Documents\\Repos\\OpenGL\\ExploreOpenGL\\resources\\textures\\VirusTube\\");
const std::string TEXTURE_DIR_MOTORBIKE("C:\\Users\\coxst\\OneDrive\\Documents\\Repos\\OpenGL\\ExploreOpenGL\\resources\\textures\\motorbike\\");

//Magic numbers
constexpr unsigned int	VERTICES_PER_TRIANGLE = 3;
const std::string		UNNAMED{ "UNNAMED" };