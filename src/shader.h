#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <glad/glad.h> // include glad to get the required OpenGL headers
#include <glm/glm.hpp>

#include "settings.h"

class Shader
{
private:
	// the program ID
	unsigned int programID;

public:
	// constructor reads and builds the shader
	Shader(const std::string& vertexPath, const std::string& fragmentPath, 
		const std::string& geometryPath = "", const std::string& shaderDir = SHADER_DIR);
	// use/activate the shader
	void use();
	// utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setFloat4(const std::string& name, float value0, float value1, float value2, float value3) const;
	void setMat2(const std::string& name, const glm::mat2& mat) const;
	void setMat3(const std::string& name, const glm::mat3& mat) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;
	void setVec2(const std::string& name, const glm::vec2& value) const;
	void setVec2(const std::string& name, float x, float y) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setVec4(const std::string& name, const glm::vec4& value) const;
	void setVec4(const std::string& name, float x, float y, float z, float w) const;

private:
	void checkCompileErrors(GLuint shader, std::string type);
};