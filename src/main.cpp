
#include <conio.h>
#include <iostream>
#include <string>
#include <vector>

//External lib
#include "glad\glad.h"
#include "GLFW\glfw3.h"
#include "KHR\khrplatform.h"
#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
void updateDeltaTime();

//Camera variables
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//Delta time variables
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

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
		Shader(shaderDir + "standardPosColor.vs", shaderDir + "colorFromVS.fs"),
		Shader(shaderDir + "standardPosColor.vs", shaderDir + "colorFromFS.fs"),
		Shader(shaderDir + "standardPosColor.vs", shaderDir + "colorFromUniform.fs"),
		Shader(shaderDir + "posColorTexture.vs", shaderDir + "colorTextureFromVS.fs"),
	};
	const unsigned int lastShaderIndex = static_cast<const unsigned int>(myShader.size() - 1);

	//Set up texture data
	unsigned int texture0, texture1;
	createTextures(resourceDir + "container.jpg", &texture0, false, GL_TEXTURE0, GL_RGB);
	createTextures(resourceDir + "awesomeface.png", &texture1, true, GL_TEXTURE1, GL_RGBA);

	// Set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	//Vertex Array
	//============
	std::vector<float> vertices = {
		//Vertex data			Color data			Texture coords 2D
		//Triangle 1
		-0.5f,  0.5f,  0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // left triangle top
		 0.0f,  0.0f,  0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f, // left triangle bottom right
		-1.0f,  0.0f,  0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, // left triangle bottom left
		//Triangle 2
		 0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // right triangle top
		 1.0f,  0.0f, 0.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f, // right triangle bottom right
		 0.0f,  0.0f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f, // right triangle bottom left
		 //Triangle 3
		 0.0f,  1.0f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // upper triangle top
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f, // upper triangle bottom right
		 0.5f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f, // upper triangle bottom left
		 //Rectangle made of two triangles used for texture mapping example
		 0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f, // bottom right
		-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f,	// top left

		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f,	// top left
		//Cube vertices (36 triangles)
		- 0.5f, -0.5f, -0.5f,	0.0f, 0.0f,	0.0f,	0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, 0.0f,	0.0f,	1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	0.0f, 0.0f,	0.0f,	1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	0.0f, 0.0f,	0.0f,	1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f, 0.0f,	0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,	0.0f,	0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,	0.0f,	0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, 0.0f,	0.0f,	1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	0.0f, 0.0f,	0.0f,	1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	0.0f, 0.0f,	0.0f,	1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	0.0f, 0.0f,	0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,	0.0f,	0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,	0.0f, 0.0f,	0.0f,	1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	0.0f, 0.0f,	0.0f,	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,	0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,	0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,	0.0f,	0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	0.0f, 0.0f,	0.0f,	1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,	0.0f, 0.0f,	0.0f,	1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	0.0f, 0.0f,	0.0f,	1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, 0.0f,	0.0f,	0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, 0.0f,	0.0f,	0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, 0.0f,	0.0f,	0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	0.0f, 0.0f,	0.0f,	1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,	0.0f,	0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, 0.0f,	0.0f,	1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, 0.0f,	0.0f,	1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, 0.0f,	0.0f,	1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,	0.0f,	0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,	0.0f,	0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,	0.0f, 0.0f,	0.0f,	0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	0.0f, 0.0f,	0.0f,	1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	0.0f, 0.0f,	0.0f,	1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	0.0f, 0.0f,	0.0f,	1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	0.0f, 0.0f,	0.0f,	0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	0.0f, 0.0f,	0.0f,	0.0f, 1.0f
	};
	std::vector<unsigned int> trianglesPerObject{1,1,1,2,12};
	constexpr unsigned int verticesPerTriangle = 3;
	constexpr unsigned int posElementsPerAttribute = 3;
	constexpr unsigned int colorElementsPerAttribute = 3;
	constexpr unsigned int textureElementsPerAttribute = 2;
	constexpr unsigned int numArrayElementsPerVertex = posElementsPerAttribute 
														+ colorElementsPerAttribute
														+ textureElementsPerAttribute;
	const unsigned int numObjects = static_cast<const unsigned int>(trianglesPerObject.size());
	
	unsigned int indices[] { // note that we start from 0
		9, 10, 12, // first triangle
		10, 11, 12 // second triangle
	};
	std::vector<unsigned int> VAO(numObjects);
	std::vector<unsigned int> VBO(numObjects);
	glGenVertexArrays(numObjects, &VAO[0]);
	glGenBuffers(numObjects, &VBO[0]);
	//glGenBuffers(1, &EBO);
	
	//Create Vertex Buffer and Vertex Array objects
	unsigned int triangleCount{ 0 };
	for (unsigned int i = 0; i < numObjects; i++) {
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO[i]);	//each call to glBindVertexArray updates VAO[i] with the reference use by OpenGL for this VAO
		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		glBufferData(GL_ARRAY_BUFFER, trianglesPerObject[i] * verticesPerTriangle * numArrayElementsPerVertex * sizeof(float),
			&vertices[triangleCount * verticesPerTriangle * numArrayElementsPerVertex], GL_STATIC_DRAW);

		triangleCount += trianglesPerObject[i];
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		GLuint location0{ 0 };
		GLuint location1{ 1 };
		GLuint location2{ 2 };
		// Set position attribute pointer
		glVertexAttribPointer(location0, posElementsPerAttribute, GL_FLOAT, GL_FALSE, numArrayElementsPerVertex * sizeof(float), (void*)0);
		glEnableVertexAttribArray(location0); //relates to the locations declared in the vertex shader

		// Set color attribute pointer
		glVertexAttribPointer(location1, colorElementsPerAttribute, GL_FLOAT, GL_FALSE, numArrayElementsPerVertex * sizeof(float),
			(void*)(posElementsPerAttribute * sizeof(float)));
		glEnableVertexAttribArray(location1); //relates to the locations declared in the vertex shader

		// Set texture attribute pointer
		glVertexAttribPointer(location2, textureElementsPerAttribute, GL_FLOAT, GL_FALSE, numArrayElementsPerVertex * sizeof(float),
			(void*)((posElementsPerAttribute + colorElementsPerAttribute)* sizeof(float)));
		glEnableVertexAttribArray(location2); //relates to the locations declared in the vertex shader
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
	glEnable(GL_DEPTH_TEST);
	// Render Loop
	// ===========
	// We don’t want the application to draw a single image and then immediately quit and close the
	// window.
	// We want the application to keep drawing images and handling user input until the program has
	// been explicitly told to stop.
	// For this reason we have to create a render loop that runs until we tell GLFW to stop.
	// The glfwWindowShouldClose function checks at the start of each loop iteration if GLFW
	// has been instructed to close.
	CursorUtil con; 
	con.cursorTo(2, 0);
	std::cout << "Any key to exit...\n";
	
	while (!glfwWindowShouldClose(window))
	{
		// An iteration of the render loop is more commonly called a frame.
		updateDeltaTime();
		printFrameRate();
		
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Draw objects
		//=============
		for (unsigned int i = 0; i < numObjects; i++) {
			glBindVertexArray(VAO[i]); 

			//First three triangles have different shaders
			unsigned int currentShader = (i < myShader.size() - 1) ? i : lastShaderIndex;
			myShader[currentShader].use();
			
			float timeValue = static_cast<float>(glfwGetTime());
			
			//Model and view matrices
			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 view = glm::mat4(1.0f);
			glm::mat4 projection;

			if (i < 3) {
				if (i == 2) {
					//Set the color of the third triangle using a uniform value
					float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
					myShader[currentShader].setFloat4("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
				}
				model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
			}
			else {
				if (i == 3) {
					//Rectangle
					model = glm::translate(model, glm::vec3(-1.5f, 0.0f, 0.0f));
					model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				}
				else if (i == 4) {
					//Cube
					model = glm::rotate(model, timeValue * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
				}
				//Set textures in rectangle and cube
				myShader[currentShader].setInt("texture0", 0); //Tell OpenGL which texture unit each shader sampler belongs to
				myShader[currentShader].setInt("texture1", 1);
			}
			//Update Look At matrix
			const float radius = 5.0f;
			float camX = sin(timeValue) * radius;
			float camY = sin(timeValue) * radius;
			float camZ = cos(timeValue) * radius;
			//lookAt takes a position, target and up vector as parameters
			view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
			projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			myShader[currentShader].setMat4("model", model);
			myShader[currentShader].setMat4("view", view);
			myShader[currentShader].setMat4("projection", projection);

			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glDrawArrays(GL_TRIANGLES, 0, verticesPerTriangle * trianglesPerObject[i]);
		}
		glBindVertexArray(0);

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
	const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) *
		cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) *
		cameraSpeed; 
}
void updateDeltaTime() {
	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}