#pragma once

#include "Shader.h"
#include <glm/glm.hpp>

enum class Material;

class Cube
{
public:
	Cube(Material material, const glm::vec3& pos);
	Cube(const Cube& cube);
	~Cube();

	void setupShader(const Shader& shader);
	void setPosition(const glm::vec3& position);
	glm::vec3 getPosition();
	static float* getVertArray();
	static int getVertCount();
	static int getVertArraySize();
	static int getIDs();
	void setLabel(const std::string& text);
	std::string& getLabel();
	void setAmbientVec(const glm::vec3& ambV);
	void setDiffuseVec(const glm::vec3& diffV);
	void setSpecularVec(const glm::vec3& specV);
	void setIsFocused(bool value);

private:
	static int id;
	constexpr static int vertCount = 216;
	static float vertices[vertCount];
	glm::vec3* position;
	glm::vec3* ambient;
	glm::vec3* diffuse;
	glm::vec3* specular;
	bool isFocused;
	float shininess;
	std::string label;
};

enum class Material
{
	Emerald,
	Jade,
	Obsidian,
	Pearl,
	Ruby,
	Turquoise,
	Brass,
	Bronze
};