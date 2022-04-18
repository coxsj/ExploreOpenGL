

#include <iostream>
#include <string>

//External lib
#include "glad\glad.h"
#include "GLFW\glfw3.h"
#include "stb_image.h"

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
void createViewPort(GLFWwindow* window, void (*resizeCallback)(GLFWwindow*, int, int)){
	// Create a Viewport
	// =================
	// Before we can start rendering we have to tell OpenGL the size of the rendering window 
	// so OpenGL knows how we want to display the data and coordinates with respect to the window.
	// We can set those dimensions via the glViewport function.
	// The first two parameters of glViewport set the location of the lower left corner of the window.
	// The third and fourth parameter set the widthand height of the rendering window in pixels,
	// which we set equal to GLFW’s window size.
	// We could actually set the viewport dimensions at values smaller than GLFW’s dimensions. Then
	// all the OpenGL rendering would be displayed in a smaller window and we could, for example,
	// display other elements outside the OpenGL viewport.
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	// Additional Viewport info
	// ------------------------
	// Behind the scenes, OpenGL uses the data specified via glViewport to transform the 2D
	// coordinates it processes to coordinates on your screen. For example, a processed point of
	// location(-0.5, 0.5) would (as its final transformation) be mapped to(200, 450) in screen coords. 
	// Processed coordinates in OpenGL are between - 1 and 1
	// We effectively map from the range (-1 to 1) to (0, 800) and (0, 600). (assuming a viewport of 800,600)

	//Resizing a window
	//=================
	// The moment a user resizes the window the viewport should be adjusted as well.
	// We can register a callback function on the window which gets called each time the window
	// is resized.This resize callback function has the following prototype:
	//	 void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	// Window resize callback function

	glfwSetFramebufferSizeCallback(window, resizeCallback);
}
GLFWwindow* createWindowObject() {

	// Create a window object
	// ======================
	// If it gives a lot of undefined reference errors, it means you didn’t
	// successfully link the GLFW library.

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return window;
	}
	// Make the context of our window the main context on the current thread
	glfwMakeContextCurrent(window);
	return window;
}
GLenum glCheckError_(const char* file, int line) {
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM: error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE: error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION: error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW: error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW: error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY: error = "OUT_OF_MEMORY"; break;
		//case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
void initializeOpenGL() {
	// We first initialize GLFW
	glfwInit();

	// Tell GLFW that 3.3 is the OpenGL version we want to use.
	// GLFW uses this info to make the proper arrangements when creating
	// the OpenGL context.
	// This ensures that when a user does not have the proper OpenGL version GLFW fails to run.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Use the core-profile so we’ll get access to a smaller subset of OpenGL features
	// without backwards - compatible features we no longer need.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // needed for apple
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}
bool loadGLFWFunctionPointers(){
	// Prepare to call OpenGL functions
	// ================================
	// Initialize GLAD (load GLFW function pointers) before we call any OpenGL functions
	// We pass GLAD the function to load the address of the OpenGL function pointers which is
	// OS - specific. GLFW gives us glfwGetProcAddress that defines the correct function based on
	// which OS we’re compiling for.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}
	return true;
}
void printOpenGLAttributes() {
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum vertex attributes supported: " << nrAttributes
		<< std::endl;
}

