#pragma once

#include <string.h>
#define glCheckError() glCheckError_(__FILE__, __LINE__)



void createTextures(const std::string& textureStr, GLuint *textureID, bool flipImage = true, GLenum activeTextureUnit= GL_TEXTURE0, GLenum format = GL_RGB);
void createViewPort(GLFWwindow* window, void (*resizeCallback)(GLFWwindow*, int, int));
GLFWwindow* createWindowObject();
GLenum glCheckError_(const char* file, int line);
void initializeOpenGL();
bool loadGLFWFunctionPointers();
void printOpenGLAttributes();