#pragma once
#include <assert.h>

//External Libraries
#include "glad\glad.h"
#include "GLFW\glfw3.h"

class Window {
private:
	static bool instantiated_;
	unsigned int scr_w_;
	unsigned int scr_h_;
	std::string name_;
	GLFWwindow* window_;
public:
	Window(const unsigned int w, const unsigned int h, const std::string& winName) :
		scr_w_(w), scr_h_(h), name_(winName), window_(nullptr)
	{
		assert(!instantiated_);
		instantiated_ = true;
	}
	~Window() { instantiated_ = false; }
	bool init();
	GLFWwindow* window() { return window_; }
private:
	static void cb_framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void createViewPort();
	bool createWindowObject();
	bool initializeOpenGL();
	bool loadGLFWFunctionPointers();
	void printOpenGLAttributes();
	void registerGLFWCallbacks();
	
};