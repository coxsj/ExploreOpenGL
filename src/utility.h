#pragma once

#include <string.h>
#define glCheckError() glCheckError_(__FILE__, __LINE__)



void createTextures(const std::string& textureStr, GLuint *textureID, GLenum activeTextureUnit= GL_TEXTURE0);
void createViewPort(GLFWwindow* window, void (*resizeCallback)(GLFWwindow*, int, int));
GLFWwindow* createWindowObject();
GLenum glCheckError_(const char* file, int line);
void initializeOpenGL();
bool loadGLFWFunctionPointers();
void printOpenGLAttributes();