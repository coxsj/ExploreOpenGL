#include <assert.h>
#include <conio.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <windows.h> //Eliminates the APIENTRY macro redefinition warning.

//External lib
#include "glad\glad.h"
#include "glfw-3.3.6\include\GLFW\glfw3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

//Local lib
#include "util_console\util_console.h"

//Project files
#include "camera.h"
#include "settings.h"
#include "shader.h"
#include "shapes.h"
#include "utility.h"
#include "window.h"

// Forward references. Definintions below main.
//=============================================
void cb_cursor_enter_callback(GLFWwindow* window, int entered);
void cb_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cb_mouse_callback(GLFWwindow* window, double xpos, double ypos);
void cb_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void createVAOAndVBOs(std::vector<unsigned int>& VAO, std::vector<unsigned int>& VBO,
	const std::vector<std::unique_ptr<NewShape>>& newShapes,
	std::vector<float>& rawVertexData);
void extractRawVertexDataAndIndices(const std::vector<std::unique_ptr<NewShape>>& newShapes,
	std::vector<Vertex>& vertices, std::vector<float>& rawVertexData,
	std::vector<unsigned int>& indices);
void printRawVertexData(const std::vector<float>& v);
void processInput(GLFWwindow* window);
void registerWindowCallbacks(GLFWwindow* window);
void updateDeltaTime();

//GLOBALS!!
//=========
std::unique_ptr<Camera> cam = std::make_unique<Camera>();
std::unique_ptr<CursorUtil> con = std::make_unique<CursorUtil>();

//Mouse control
bool mouseHover{ false };
bool mouseLeftDrag{ false };
bool mouseRightDrag{ false };

//Delta time variables
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

int main()
{
	Window win(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL");
	if (!win.init()) return -1;

	registerWindowCallbacks(win.window());

	//Set up texture data
	const std::vector<TextureFileData> textureFiles{
		{"container.jpg",			false, GL_RGB},
		{"awesomeface.png",			true,  GL_RGBA},
		{"container2.png",			false, GL_RGBA},
		{"container2_specular.png", false, GL_RGBA} };
	std::vector<unsigned int> textureIDs(textureFiles.size());
	createTextures(textureFiles, textureIDs);
	
	//Build and compile Shader Programs
	std::vector<Shader> myShader{
		Shader{"standardPosColor.vs", "colorFromVS.fs"},
		Shader{"standardPosColor.vs", "colorFromFS.fs"},
		Shader{"standardPosColor.vs", "colorFromUniform.fs" },
		Shader{"posColorTexture.vs", "colorTextureFromVS.fs"},
		Shader{"lighting.vs", "lighting.fs"},
		Shader{"lighting.vs", "light.fs"},
		Shader{"standard.vs", "standard.fs"},
		Shader{"standard.vs", "standardfxn.fs"}
	};

	// Vertex data ranges, indices and offsets
	
	glCheckError();

	// Uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);

	// Render Loop
	// ===========
	// The render loop runs until we tell GLFW to stop.
	// The glfwWindowShouldClose function checks at the start of each loop iteration if GLFW
	// has been instructed to close.
	con->cursorTo(2, 0);
	std::cout << "Any key to exit...\n";
	while (!glfwWindowShouldClose(win.window()))
	{
		// An iteration of the render loop is more commonly called a frame.
		updateDeltaTime();
		printFrameRate();
		
		//Check for user input
		processInput(win.window());
		
		// Render Commands
		// ===============
		// Set the color used by glClear to fill the color buffer.	
		glClearColor(0.02f, 0.03f, 0.03f, 1.0f);
		// Clear the screen buffer. Available bits we can set are 
		// GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT and GL_STENCIL_BUFFER_BIT.
		// glClearColor is a state-setting function. glClear is a state-using function
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Draw objects
		//=============
		for (unsigned int i = 0; i < 0; i++) {
			//Select and activate shader program
			unsigned int currentShader = 0;
			//assert(currentShader < myShader.size());
			//myShader[currentShader].use();
			//Bind VAO for this shape
			//glBindVertexArray(VAO[i]);
			//Model and view matrices
			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 view = glm::mat4(1.0f);
			glm::mat4 projection = glm::mat4(1.0f);
			//Update model matrix
			float timeValue = static_cast<float>(glfwGetTime());
			float greenValue;
			glm::vec3 lightSourcePos(1.5f * glm::cos(timeValue/10.0), 
				1.5f * glm::sin(timeValue/10.0), 4.0f);
			glm::vec3 lightSourceColor{ 1.0f, 1.0f, 1.0f };
			unsigned int pos{0};
			switch (i) {
			case 0:
				model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
				myShader[currentShader].setInt("material.colorType", VERTEX_COLOR);
				break;
			case 1:
				model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
				myShader[currentShader].setInt("material.colorType", OBJECT_COLOR);
				myShader[currentShader].setFloat4("material.objectColor", 0.75f, 0.75f, 0.0f, 1.0f);
				break;
			case 2:
				model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
				//Set the color of the third triangle using a uniform value
				myShader[currentShader].setInt("material.colorType", OBJECT_COLOR);
				greenValue = (sin(timeValue) / 2.0f) + 0.5f;
				myShader[currentShader].setFloat4("material.objectColor", 0.0f, greenValue, 0.0f, 1.0f);
				break;
			case 3:
				//Rectangle
				model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::translate(model, glm::vec3(-1.5f, 0.0f, 0.0f));
				//Set textures in rectangle and cube
				myShader[currentShader].setInt("material.colorType", TEXTURE_COLOR);
				myShader[currentShader].setInt("material.tA", 0);
				myShader[currentShader].setInt("material.tB", 1);
				break;
			case 4:
				//Cube with difuse and specular maps
				model = glm::translate(model, glm::vec3(-1.0f, -1.5f, 0.0f));
				model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
				model = glm::rotate(model, timeValue * glm::radians(50.0f), 
					glm::vec3(0.5f, 1.0f, 0.0f));
				//Set textures in cube
				myShader[currentShader].setInt("material.colorType", LIGHTMAP_COLOR);
				myShader[currentShader].setInt("material.diffuse", 2);
				myShader[currentShader].setInt("material.specular", 3);
				break;
			case 5:
				//Cube
				model = glm::translate(model, glm::vec3(2.0f, -1.0f, 0.0f));
				model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
				model = glm::rotate(model, -timeValue * glm::radians((i - 3) * 17.0f), 
					glm::vec3(-0.5f, 1.0f, 0.0f));
				//Set object color
				myShader[currentShader].setInt("material.colorType", OBJECT_COLOR);
				myShader[currentShader].setFloat4("material.objectColor", 1.0f, 0.5f, 0.0f, 1.0f);
				break;
			case 6:
				//Cube - Light source
				model = glm::translate(model, lightSourcePos);
				model = glm::scale(model, glm::vec3(0.2f));
				model = glm::rotate(model, -timeValue * glm::radians(25.0f), 
					glm::vec3(-0.5f, 1.0f, 1.0f));
				myShader[currentShader].setVec3("lightSourceColor", lightSourceColor);
				break;
			case 7: case 8: case 9: case 10:

				break;
			default:
				assert(false);
				break;
			}
			glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
			myShader[currentShader].setMat3("normalMatrix", normalMatrix);
			//Set lighting uniforms
			//=====================
			myShader[currentShader].setVec3("viewPos", cam->getPos());

			
			//Update camera view
			view = cam->lookAt();
			projection = cam->perspective(SCR_WIDTH, SCR_HEIGHT, 0.1f, 100.0f);
			//Apply current transformation matrices to shader uniforms
			myShader[currentShader].setMat4("model", model);
			myShader[currentShader].setMat4("view", view);
			myShader[currentShader].setMat4("projection", projection);
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			//glDrawArrays(GL_TRIANGLES, 0, VERTICES_PER_TRIANGLE * newShapes[i]->size());	
			glCheckError();
		}
		glBindVertexArray(0);
		
		// Swap Buffers
		// ============
		// glfwSwapBuffers swaps the color buffer (a large 2D buffer that contains color values 
		// for each pixel in GLFW�s window) that is used to render to during this render 
		// iteration and show it as output to the screen.
		glfwSwapBuffers(win.window());

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
void cb_cursor_enter_callback(GLFWwindow* window, int entered) {
	if (entered) {
		// The cursor entered the content area of the window
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		mouseHover = true;
	}
	else {
		// The cursor left the content area of the window
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		mouseHover = false;
	}
}
void cb_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (!mouseHover) return;
	if (button == GLFW_MOUSE_BUTTON_LEFT ){
		if (action == GLFW_PRESS) {
			if (!mouseLeftDrag) mouseLeftDrag = true;
		}
		if (action == GLFW_RELEASE) {
			if (mouseLeftDrag) mouseLeftDrag = false;
		}
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS) {
			if (!mouseRightDrag) mouseRightDrag = true;
		}
		if (action == GLFW_RELEASE) {
			if (mouseRightDrag) mouseRightDrag = false;
		}
	}
}
void cb_mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	// Mouse variables
	static float lastX, lastY;
	static bool firstMouse{ true };
	if (!mouseHover) return;
	if (!mouseLeftDrag) {
		firstMouse = true;
		return;
	}
	if (firstMouse) // Sidesteps mouse jump on entry
	{
		lastX = static_cast<float>(xpos);
		lastY = static_cast<float>(ypos);
		firstMouse = false;
	}
	float xoffset = static_cast<float>(xpos - lastX);
	float yoffset = static_cast<float>(lastY - ypos); // reversed: y ranges bottom to top
	lastX = static_cast<float>(xpos);
	lastY = static_cast<float>(ypos);

	cam->updateDirectionFromMouse(xoffset, yoffset);
}
void cb_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (!mouseHover) return;
	cam->newZoom(static_cast<float>(yoffset));
}
void createVAOAndVBOs(std::vector<unsigned int>& VAO, std::vector<unsigned int>& VBO,
	const std::vector<std::unique_ptr<NewShape>>& newShapes,
	std::vector<float>& rawVertexData) {

	//unsigned int EBO;
	const unsigned int numObjects = static_cast<const unsigned int>(newShapes.size());
	glGenVertexArrays(numObjects, &VAO[0]);
	glGenBuffers(numObjects, &VBO[0]);
	//glGenBuffers(1, &EBO);

	unsigned int dataOffset{ 0 };
	for (unsigned int i = 0; i < numObjects; i++) {
		// Bind the Vertex Array Object first, then bind vertex buffer(s),
		// and then configure vertex attributes(s).
		glBindVertexArray(VAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		//glBufferData(GL_ARRAY_BUFFER, sizeofmydata, myData, usage);
		unsigned int dataSize = newShapes[i]->size() * VERTICES_PER_TRIANGLE * sizeof(Vertex);
		glBufferData(GL_ARRAY_BUFFER, dataSize, &rawVertexData[dataOffset], GL_STATIC_DRAW);
		dataOffset += dataSize / sizeof(float);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		GLuint location0{ 0 };
		GLuint location1{ 1 };
		GLuint location2{ 2 };
		GLuint location3{ 3 };
		// Set position attribute pointer
		// glVertexAttribPointer(location, elements per attribute, type of data, bool, sizeof vertex, pointer to first data element)
		//glVertexAttribPointer(location0, posElementsPerAttribute, GL_FLOAT, GL_FALSE, numArrayElementsPerVertex * sizeof(float), (void*)0);
		glVertexAttribPointer(location0, Vertex::posLength(), GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(void*)OFFSET_POS);
		glEnableVertexAttribArray(location0); //relates to the locations declared in the vertex shader

		// Set texture attribute pointer
		glVertexAttribPointer(location2, Vertex::textureCoordLength(), GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(void*)OFFSET_TEXTURECOORD);
		glEnableVertexAttribArray(location2); //relates to the locations declared in the vertex shader

		// Set normals attribute pointer
		glVertexAttribPointer(location3, Vertex::normalLength(), GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(void*)OFFSET_NORMALS);
		glEnableVertexAttribArray(location3); //relates to the locations declared in the vertex shader
	}
	// The call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex 
	// buffer object so afterwards we can safely unbind.
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, 
	// but this rarely happens. Modifying other VAOs requires a call to glBindVertexArray 
	// anyway, so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
}
void extractRawVertexDataAndIndices(const std::vector<std::unique_ptr<NewShape>>& newShapes,
	std::vector<Vertex>& vertices, std::vector<float>& rawVertexData,
	std::vector<unsigned int>& indices) {
	for (auto& s : newShapes) {
		//std::cout << "Shape\n";
		for (unsigned int i = 0; i < s->vertexCount(); i++) {
			//Loop through vertices in each triangle
			//Add vertex data to rawVertexData
			s->vertex(i).add(rawVertexData);
			//Loop through all vertices and capture the index of this vertex
			bool found = false;
			for (unsigned int k = 0; k < vertices.size(); k++) {
				if (s->vertex(i) == vertices[k]) {
					indices.push_back(k);
					found = true;
					break;
				}
			}
			assert(found);
		}
	}
	//printRawVertexData(rawVertexData);
	//for (auto i : indices) std::cout << i << ", ";
}
void printRawVertexData(const std::vector<float>& v) {
	unsigned int ctr{ 0 };
	for (auto a : v) {
		std::cout << a << ",";
		if (++ctr >= sizeof(Vertex) / sizeof(float)) {
			std::cout << "\n";
			ctr = 0;
		}
	}
}
void processInput(GLFWwindow* window){
	// Check if escape key pressed (if it�s not pressed, glfwGetKey returns GLFW_RELEASE)
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//Check for keyboard camera control input
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cam->keyInput(GLFW_KEY_W, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cam->keyInput(GLFW_KEY_S, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cam->keyInput(GLFW_KEY_A, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cam->keyInput(GLFW_KEY_D, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) cam->keyInput(GLFW_KEY_UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) cam->keyInput(GLFW_KEY_DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) cam->keyInput(GLFW_KEY_LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) cam->keyInput(GLFW_KEY_RIGHT, deltaTime);
}
void registerWindowCallbacks(GLFWwindow* window) {
	glfwSetMouseButtonCallback(window, cb_mouse_button_callback);
	glfwSetCursorEnterCallback(window, cb_cursor_enter_callback);
	glfwSetCursorPosCallback(window, cb_mouse_callback);
	glfwSetScrollCallback(window, cb_scroll_callback);
}
void updateDeltaTime() {
	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}