#pragma once

#include <string.h>
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void createViewPort(GLFWwindow* window, void (*resizeCallback)(GLFWwindow*, int, int));
GLFWwindow* createWindowObject();
GLenum glCheckError_(const char* file, int line);
void initializeOpenGL();
bool loadGLFWFunctionPointers();
void printOpenGLAttributes();