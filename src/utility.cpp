
//#define NDEBUG
#include <assert.h>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

//External lib
#include "glad\glad.h"
#include "glfw-3.3.6\include\GLFW\glfw3.h"
#include "stb_image.h"

//Local lib
#include "util_console/util_console.h"

#include "settings.h"
#include "utility.h"

void createTextures(const std::vector<TextureFileData>& textureFiles, std::vector<unsigned int>& textureIDs,
	const std::string resDir) {
	unsigned int numTexturesToLoad = static_cast<unsigned int>(textureFiles.size());
	glGenTextures(numTexturesToLoad, &textureIDs[0]);
	for (unsigned int i = 0; i < numTexturesToLoad; i++) {
		glActiveTexture(GL_TEXTURE0 + i);	// Activate texture unit first. Note that GL_TEXTURE0 is always active
										// If you glActiveateTexture without binding a texture unit
										// it will be bound to GL_TEXTURE0 by default
		glBindTexture(GL_TEXTURE_2D, textureIDs[i]);
		// set the texture wrapping/filtering options (on currently bound texture)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load and generate the texture
		if (textureFiles[i].flipImage) stbi_set_flip_vertically_on_load(true);
		int width, height, nrChannels;
		unsigned char* data = stbi_load((resDir + textureFiles[i].filename).c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, textureFiles[i].format,
				GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else std::cout << "Failed to load texture " << textureFiles[i].filename << std::endl;
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
unsigned int textureFromFile(const char* path, const std::string& directory, bool gamma) {
	//TODO factor improvements and adjustments from utility loadTexture
	std::string filename = std::string(path);
	filename = directory + '/' + filename;
	unsigned int textureID;
	glGenTextures(1, &textureID);
	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
	}
	else {
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}


