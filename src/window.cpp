#include <iostream>

#include "window.h"

//Definiiton of static members must appear in the cpp file
bool Window::instantiated_=false;

void Window::cb_framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// This callback makes sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
void Window::createViewPort() {
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
	glViewport(0, 0, scr_w_, scr_h_);

	// Additional Viewport info
	// ------------------------
	// Behind the scenes, OpenGL uses the data specified via glViewport to transform the 2D
	// coordinates it processes to coordinates on your screen. For example, a processed point of
	// location(-0.5, 0.5) would (as its final transformation) be mapped to(200, 450) in screen coords. 
	// Processed coordinates in OpenGL are between - 1 and 1
	// We effectively map from the range (-1 to 1) to (0, 800) and (0, 600). (assuming a viewport of 800,600)
}
bool Window::createWindowObject() {

	// Create a window object
	// ======================
	// If it gives a lot of undefined reference errors, it means you didn’t
	// successfully link the GLFW library.

	window_ = glfwCreateWindow(scr_w_, scr_h_, name_.c_str(), NULL, NULL);
	if (window_ == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		return false;
	}
	// Make the context of our window the main context on the current thread
	glfwMakeContextCurrent(window_);
	return true;
}
bool Window::initializeOpenGL() {
	// First initialize GLFW
	if (glfwInit() == GLFW_FALSE) {
		std::cout << "Failed to initialize OpenGL" << std::endl;
		return false;
	}

	// Tell GLFW that 3.3 is the OpenGL version we want to use.
	// GLFW uses this info to make the proper arrangements when creating the OpenGL context.
	// This ensures that when a user does not have the proper OpenGL version GLFW fails to run.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Use the core-profile so we’ll get access to a smaller subset of OpenGL features
	// without backwards - compatible features we no longer need.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// needed for apple
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	return true;
}
bool Window::init() {
	if(!initializeOpenGL()) return false;

	if (!createWindowObject()) {
		glfwTerminate();
		return false;
	}
	if (!loadGLFWFunctionPointers()) return false;
	printOpenGLAttributes();
	createViewPort();
	registerGLFWCallbacks();
	return true;
}
bool Window::loadGLFWFunctionPointers() {
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
void Window::printOpenGLAttributes() {
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum vertex attributes supported: " << nrAttributes << std::endl;
}
void Window::registerGLFWCallbacks() {
	//Resizing a window
	//=================
	// The moment a user resizes the window the viewport should be adjusted as well.
	// We can register a callback function on the window which gets called each time the window
	// is resized.This resize callback function has the following prototype:
	//	 void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	// Window resize callback function

	glfwSetFramebufferSizeCallback(window_, cb_framebuffer_size_callback);
}
