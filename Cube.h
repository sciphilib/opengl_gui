#pragma once

#include "Shader.h"
#include <glm/glm.hpp>

enum class Material;

class Cube
{
public:
	Cube(Material material);
	Cube(const Cube& cube);
	~Cube();
	void setupShader(const Shader& shader);
	static float* getVertArray();
	void setPosition(const glm::vec3& position);
	glm::vec3 getPosition();
	static int getVertCount();
	static int getVertArraySize();
private:
	constexpr static int vertCount = 216;
	static float vertices[vertCount];
	glm::vec3* position;
	glm::vec3* ambient;
	glm::vec3* diffuse;
	glm::vec3* specular;
	float shininess;
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