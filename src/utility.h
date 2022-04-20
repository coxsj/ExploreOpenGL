#pragma once

#include <string.h>

#define glCheckError() glCheckError_(__FILE__, __LINE__)

void createTextures(const std::string& textureStr, GLuint *textureID, bool flipImage = true, GLenum activeTextureUnit= GL_TEXTURE0, GLenum format = GL_RGB);
GLenum glCheckError_(const char* file, int line);
bool loadGLFWFunctionPointers();
void printFrameRate();
