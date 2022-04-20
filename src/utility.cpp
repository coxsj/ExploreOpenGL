
//#define NDEBUG
#include <assert.h>
#include <chrono>
#include <iostream>
#include <string>

//External lib
#include "glad\glad.h"
#include "GLFW\glfw3.h"
#include "stb_image.h"

//Local lib
#include "util_console/util_console.h"

#include "settings.h"
#include "utility.h"

void createTextures(const std::string& textureStr, GLuint *textureID, bool flipImage, GLenum activeTextureUnit, GLenum format) {
	unsigned int numTexturesToLoad = 1;
	glGenTextures(numTexturesToLoad, textureID);
	glActiveTexture(activeTextureUnit);	// Activate texture unit first. Note that GL_TEXTURE0 is always active
									// If you glActiveateTexture without binding a texture unit
									// it will be bound to GL_TEXTURE0 by default
	glBindTexture(GL_TEXTURE_2D, *textureID);
	// set the texture wrapping/filtering options (on currently bound texture)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	if(flipImage) stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char* data = stbi_load(textureStr.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
}
GLenum glCheckError_(const char* file, int line) {
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:		error = "INVALID_ENUM";		break;
		case GL_INVALID_VALUE:		error = "INVALID_VALUE";	break;
		case GL_INVALID_OPERATION:	error = "INVALID_OPERATION";break;
		case GL_STACK_OVERFLOW:		error = "STACK_OVERFLOW";	break;
		case GL_STACK_UNDERFLOW:	error = "STACK_UNDERFLOW";	break;
		case GL_OUT_OF_MEMORY:		error = "OUT_OF_MEMORY";	break;
		//case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
void printFrameRate() {
	static long long loopCtr{ 0 };

	static auto t_start{ std::chrono::high_resolution_clock::now() };
	static auto t_end{ std::chrono::high_resolution_clock::now() };
	static double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
	CursorUtil con; 
	con.cursorTo(1, 0);
	t_end = std::chrono::high_resolution_clock::now();
	elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
	t_start = t_end;
	std::cout << loopCtr++ << " " << 1000 / elapsed_time_ms;
}


