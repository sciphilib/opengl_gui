#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int ID; // ID - the program id

	// constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);

	// use/activate the shader
	void use();

	// utility uniform-functions
	void setBool	(const std::string& name, bool	value) const;
	void setInt		(const std::string& name, int	value) const;
	void setFloat	(const std::string& name, float value) const;
	void setMat4	(const std::string& name, const glm::mat4& mat) const;
	void setVec3	(const std::string& name, float x, float y, float z) const;
	void setVec3	(const std::string& name, const glm::vec3& vec) const;
	//void setVec3fv	(const std::string& name, const glm::vec3& vec) const;
};
