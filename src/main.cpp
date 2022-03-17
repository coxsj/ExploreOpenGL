#include <iostream>
#include <conio.h>

#include "glad\glad.h"
#include "GLFW\glfw3.h"
#include "KHR\khrplatform.h"
#include "utility.h"

// Forward references. Definintions below main.
unsigned int createFragmentShader(const char* fShaderSource);
unsigned int createShaderProgram(const unsigned int vertexShader, const unsigned int fragmentShader);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//Shader program strings
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSourceOrange = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

const char* fragmentShaderSourcePink = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.898f, 0.180f, 0.886f, 1.0f);\n"
"}\n\0";

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
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create a window object
	// ======================
	// If it gives a lot of undefined reference errors, it means you didn’t
	// successfully link the GLFW library.

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Make the context of our window the main context on the current thread
	glfwMakeContextCurrent(window);

	// Prepare to call OpenGL functions
	// ================================
	// Initialize GLAD (load GLFW function pointers) before we call any OpenGL functions
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
	// We effectively map from the range (-1 to 1) to (0, 800) and (0, 600). (ssuming a viewport of 800,600)

	//Resizing a window
	//=================
	// The moment a user resizes the window the viewport should be adjusted as well.
	// We can register a callback function on the window which gets called each time the window
	// is resized.This resize callback function has the following prototype:
	//	 void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Build and compile Shader Program
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shaders
	const unsigned int numFragShaders = 2;
	unsigned int fragmentShader[numFragShaders];
	fragmentShader[0] = createFragmentShader(fragmentShaderSourceOrange);
	fragmentShader[1] = createFragmentShader(fragmentShaderSourcePink);

	// link shaders
	unsigned int shaderProgram[numFragShaders];
	for (auto i = 0; i < numFragShaders; i++) {
		shaderProgram[i] = createShaderProgram(vertexShader, fragmentShader[i]);
	}
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader[0]);
	glDeleteShader(fragmentShader[1]);

	// Set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	//Vertex Array
	//============
	float vertices[] = {
		-0.5f,  0.5f, 0.0f, // left triangle top
		 0.0f,  0.0f, 0.0f, // left triangle bottom right
		-1.0f,  0.0f, 0.0f, // left triangle bottom left
		 0.5f,  0.5f, 0.0f, // right triangle top
		 1.0f,  0.0f, 0.0f, // right triangle bottom right
		 0.0f,  0.0f, 0.0f  // right triangle bottom left
	};
	unsigned int indices[] = { // note that we start from 0!
		0, 1, 3, // first triangle
		1, 2, 3 // second triangle
	};

	const unsigned int numTriangles = 2;
	unsigned int VAO[numTriangles], VBO[numTriangles];
	glGenVertexArrays(numTriangles, VAO);
	glGenBuffers(numTriangles, VBO);
	//glGenBuffers(1, &EBO);
	// 
	for (auto i = 0; i < numTriangles; i++) {
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*9, vertices+i*9, GL_STATIC_DRAW);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Set attribute pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}
	// The call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex 
	// buffer object so afterwards we can safely unbind.
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, 
	// // but this rarely happens. Modifying other VAOs requires a call to glBindVertexArray 
	// anyway, so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	glCheckError();

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Render Loop (Basic)
	// ==================
	// We don’t want the application to draw a single image and then immediately quit and close the
	// window.
	// We want the application to keep drawing images and handling user input until the program has
	// been explicitly told to stop.
	// For this reason we have to create a render loop that runs until we tell GLFW to stop.
	// The glfwWindowShouldClose function checks at the start of each loop iteration if GLFW
	// has been instructed to close.
	std::cout << "Any key to exit...\n";
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
		// glClearColor is a state-setting function
		// glClear is a state-using function
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		// draw our first triangle
		
		for (auto i = 0; i < numTriangles; i++) {
			glUseProgram(shaderProgram[i]);
			glBindVertexArray(VAO[i]); 

			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glDrawArrays(GL_TRIANGLES, 0, 3); //Not needed if using Element Buffer Object
		}
		glBindVertexArray(0); // Dont have to unbind it every time, esp when only one object to draw

		//Check for glErrors
		glCheckError();
		
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
			instant, but drawn pixel by pixel and usually from left to right, top to bottom.
			Because this image is not displayed at an instant to the user while still being rendered 
			to, the result may contain artifacts.
			To circumvent these issues, windowing applications apply a double buffer for rendering.
			The front buffer contains the final output image that is shown on the screen, while all 
			the rendering commands draw to the back buffer. As soon as all the rendering commands are
			finished, we swap the back buffer to the front buffer so the image can be displayed
			without still being rendered to, removing all the aforementioned
			artifacts.
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

unsigned int createFragmentShader(const char* fShaderSource) {
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return fragmentShader;
}
// Window resize callback function
// We do have to tell GLFW we want to call this function on every window resize by registering it:
// glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
// Note: For retina displays width and height will end up significantly higher than the original 
// input values.

unsigned int createShaderProgram(const unsigned int vertexShader, const unsigned int fragmentShader) {
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	return shaderProgram;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// This callback makes sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	// Check if escape key pressed (if it’s not pressed, glfwGetKey returns GLFW_RELEASE)
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}