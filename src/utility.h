#pragma once

#include <string.h>

#define glCheckError() glCheckError_(__FILE__, __LINE__)

void createTextures(const std::vector<TextureFileData>& textureFiles, std::vector<unsigned int>& textureIDs,
	const std::string resDir = RESOURCE_DIR);
GLenum glCheckError_(const char* file, int line);
bool loadGLFWFunctionPointers();
void printFrameRate();
