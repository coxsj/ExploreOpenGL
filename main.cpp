#include <iostream>
#include <conio.h>

#include "glad\glad.h"
#include "GLFW\glfw3.h"
#include "KHR\khrplatform.h"

// Forward references. Definintions below main.
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
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

	// Create a window object
	// ======================
	// If it gives a lot of undefined reference errors, it means you didn’t
	// successfully link the GLFW library.

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Make the context of our window the main context on the current thread
	glfwMakeContextCurrent(window);

	//Initialize GLAD before we call any OpenGL functions
	// We pass GLAD the function to load the address of the OpenGL function pointers which is
	// OS - specific. GLFW gives us glfwGetProcAddress that defines the correct function based on
	// which OS we’re compiling for.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Create a Viewport
	// =================
	// Before we can start rendering we have to do one last thing
	// We have to tell OpenGL the size of the rendering window so OpenGL knows how we want 
	// to display the data and coordinates with respect to the window.
	// We can set those dimensions via the glViewport function.
	// The first two parameters of glViewport set the location of the lower left corner of the window.
	// The thirdand fourth parameter set the widthand height of the rendering window in pixels,
	// which we set equal to GLFW’s window size.
	// We could actually set the viewport dimensions at values smaller than GLFW’s dimensions. Then
	// all the OpenGL rendering would be displayed in a smaller window and we could, for example,
	// display other elements outside the OpenGL viewport.
	glViewport(0, 0, 800, 600);

	// Additional Viewport info
	// ------------------------
	// Behind the scenes, OpenGL uses the data specified via glViewport to transform the 2D
	// coordinates it processes to coordinates on your screen. For example, a processed point of
	// location(-0.5, 0.5) would(as its final transformation) be mapped to(200, 450) in screen
	// coordinates. Note that processed coordinates in OpenGL are between - 1 and 1 so we
	// effectively map from the range(-1 to 1) to(0, 800) and (0, 600).

	//Resizing a window
	//=================
	// The moment a user resizes the window the viewport should be adjusted as well.
	// We can register a callback function on the window which gets called each time the window
	// is resized.This resize callback function has the following prototype:
	// void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Render Loop (Basic)
	// ==================
	// We don’t want the application to draw a single image and then immediately quit and close the
	// window.
	// We want the application to keep drawing images and handling user input until the program has
	// been explicitly told to stop.
	// For this reason we have to create a render loop that runs until we tell GLFW to stop.
	// The glfwWindowShouldClose function checks at the start of each loop iteration if GLFW
	// has been instructed to close.
	while (!glfwWindowShouldClose(window))
	{
		// An iteration of the render loop is more commonly called a frame.

		//Check for user input
		processInput(window);
		
		// Render Commands
		// ===============

		// First clear the screen’s color buffer using glClear where we pass in buffer bits 
		// to specify which buffer we would like to clear.The possible bits we can set are 
		// GL_COLOR_BUFFER_BIT, 
		// GL_DEPTH_BUFFER_BIT and 
		// GL_STENCIL_BUFFER_BIT.
		// Note that we also specify the color to clear the screen with using glClearColor.
		// Whenever we call glClear and clear the color buffer, the entire color buffer will be 
		// filled with the color as	configured by glClearColor.
		// glClearColor is a statesetting function
		// glClear is a state-using function
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		

		// Swap Buffers
		// ============
		// glfwSwapBuffers swaps the color buffer (a large 2D buffer that contains color values 
		// for each pixel in GLFW’s window) that is used to render to during this render 
		// iteration and show it as output to the screen.
		glfwSwapBuffers(window);

		// Additional Swap Buffer info
		// ===========================
		/*  Double buffer When an application draws in a single buffer the resulting image may
			display flickering issues.This is because the resulting output image is not drawn in an
			instant, but drawn pixel by pixeland usually from left to rightand top to bottom.Because
			this image is not displayed at an instant to the user while still being rendered to, the result
			may contain artifacts.To circumvent these issues, windowing applications apply a double
			buffer for rendering.The front buffer contains the final output image that is shown at
			the screen, while all the rendering commands draw to the back buffer.As soon as all
			the rendering commands are finished we swap the back buffer to the front buffer so the
			image can be displayed without still being rendered to, removing all the aforementioned
			artifacts
		*/

		// glfwPollEvents checks if any events are triggered, like keyboard input or mouse movement
		// events, updates the window state, or calls the corresponding functions (which we can 
		// register via callback methods).
		glfwPollEvents();
	}

	// Clean / delete all of GLFW’s resources that were allocated.
	glfwTerminate();

	char z = _getch();
	return 0;
}

// Window resize callback function
// We do have to tell GLFW we want to call this function on every window resize by registering it:
// glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
// Note: For retina displays width and height will end up significantly higher than the original 
// input values.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	// Check if escape key pressed (if it’s not pressed, glfwGetKey returns GLFW_RELEASE)
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}