#pragma once

#include <iostream>

class Texture
{
public:
	unsigned int id;
	std::string type;
	std::string path; // store path of texture to compare with other textures
};

