#include <chrono>
#include <conio.h>
#include <iostream>
#include <vector>

//External lib
#include "glad\glad.h"
#include "GLFW\glfw3.h"
#include "KHR\khrplatform.h"

//Local lib
#include "util_console/util_console.h"

//Project files
#include "shader.h"
#include "settings.h"
#include "utility.h"

// Forward references. Definintions below main.
//=============================================
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

CursorUtil con;

int main()
{
	initializeOpenGL();

	GLFWwindow* window = createWindowObject();
	if(window == NULL) return -1;

	if(!loadGLFWFunctionPointers()) return -1;
	printOpenGLAttributes();

	createViewPort(window, framebuffer_size_callback);

	//Build and compile Shader Programs
	std::vector<Shader> myShader {
		Shader(workingDir + "standardPosColor.vs", workingDir + "colorFromVS.fs"),
		Shader(workingDir + "standardPosColor.vs", workingDir + "colorFromFS.fs"),
		Shader(workingDir + "standardPosColor.vs", workingDir + "colorFromUniform.fs") 
	};

	// Set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	//Vertex Array
	//============
	std::vector<float> vertices {
		//Vertex data			Color data
		-0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	// left triangle top
		 0.0f,  0.0f, 0.0f,		0.0f, 1.0f, 0.0f,	// left triangle bottom right
		-1.0f,  0.0f, 0.0f,		0.0f, 0.0f, 1.0f,	// left triangle bottom left
		 0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	// right triangle top
		 1.0f,  0.0f, 0.0f,		0.0f, 1.0f, 0.0f,	// right triangle bottom right
		 0.0f,  0.0f, 0.0f,		0.0f, 0.0f, 1.0f,	// right triangle bottom left
		 0.0f,  1.0f, 0.0f,		1.0f, 0.0f, 0.0f,	// upper triangle top
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	// upper triangle bottom right
		 0.5f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f	// upper triangle bottom left
	};
	constexpr unsigned int posElementsPerAttribute = 3;
	constexpr unsigned int colorElementsPerAttribute = 3;
	constexpr unsigned int numArrayElementsPerVertex = posElementsPerAttribute + colorElementsPerAttribute;
	unsigned int indices[] { // note that we start from 0!
		0, 1, 3, // first triangle
		1, 2, 3 // second triangle
	};

	const unsigned int numTriangles = 3;
	unsigned int VAO[numTriangles], VBO[numTriangles];
	glGenVertexArrays(numTriangles, VAO);
	glGenBuffers(numTriangles, VBO);
	//glGenBuffers(1, &EBO);
	
	//Create Vertex Buffer and Vertex Array objects
	for (auto i = 0; i < numTriangles; i++) {
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		glBufferData(GL_ARRAY_BUFFER, numTriangles * numArrayElementsPerVertex * sizeof(float), 
			&vertices[i * numTriangles * numArrayElementsPerVertex], GL_STATIC_DRAW);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Set position attribute pointer
		glVertexAttribPointer(0, posElementsPerAttribute, GL_FLOAT, GL_FALSE, numArrayElementsPerVertex * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Set color attribute pointer
		glVertexAttribPointer(1, colorElementsPerAttribute, GL_FLOAT, GL_FALSE, numArrayElementsPerVertex * sizeof(float),
			(void*)(posElementsPerAttribute * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	// The call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex 
	// buffer object so afterwards we can safely unbind.
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, 
	// but this rarely happens. Modifying other VAOs requires a call to glBindVertexArray 
	// anyway, so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	glCheckError();

	// Uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Render Loop
	// ===========
	// We don�t want the application to draw a single image and then immediately quit and close the
	// window.
	// We want the application to keep drawing images and handling user input until the program has
	// been explicitly told to stop.
	// For this reason we have to create a render loop that runs until we tell GLFW to stop.
	// The glfwWindowShouldClose function checks at the start of each loop iteration if GLFW
	// has been instructed to close.
	con.cursorTo(2, 0);	
	std::cout << "Any key to exit...\n";
	long long loopCtr = 0;

	auto t_start = std::chrono::high_resolution_clock::now();
	auto t_end = std::chrono::high_resolution_clock::now();
	double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
	while (!glfwWindowShouldClose(window))
	{
		// An iteration of the render loop is more commonly called a frame.
		con.cursorTo(1, 0);
		t_end = std::chrono::high_resolution_clock::now();
		elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
		t_start = t_end;
		std::cout << loopCtr++ << " " << 1000/elapsed_time_ms; 

		//Check for user input
		processInput(window);
		
		// Render Commands
		// ===============

		// First clear the screen�s color buffer using glClear where we pass in buffer bits 
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
		
		// Draw triangles
		for (auto i = 0; i < myShader.size(); i++) {
			myShader[i].use();
			myShader[i].setFloat("offset", 0.0f);
			if (i == 1) {
				//Send offset value via uniform to Vertex shader
				myShader[i].setFloat("offset", 0.5f);
			}
			else if (i == 2) {
				float timeValue = glfwGetTime();
				float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
				myShader[2].setFloat4("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
			}
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
		// for each pixel in GLFW�s window) that is used to render to during this render 
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

	// Clean / delete all of GLFW�s resources that were allocated.
	glfwTerminate();

	char z = _getch();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// This callback makes sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
	// Check if escape key pressed (if it�s not pressed, glfwGetKey returns GLFW_RELEASE)
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}