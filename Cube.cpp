#include "Cube.h"

Cube::Cube(Material material)
{
	std::cout << "Cube was created by constructor\n";
	position = new glm::vec3();
	switch (material)
	{
	case Material::Emerald:
		ambient   = new glm::vec3(0.0215f, 0.1745f, 0.0215f);
		diffuse   = new glm::vec3(0.07568f, 0.61424f, 0.07568f);
		specular  = new glm::vec3(0.633f, 0.727811f, 0.633f);
		shininess = 0.6f;
		break;
	case Material::Jade:
		ambient   = new glm::vec3(0.135f, 0.2225f, 0.1575f);
		diffuse   = new glm::vec3(0.54f, 0.89f, 0.63f);
		specular  = new glm::vec3(0.316228f, 0.316228f, 0.316228f);
		shininess = 0.1f;
		break;
	case Material::Obsidian:
		ambient  = new glm::vec3(0.05375f, 0.05f, 0.06625f);
		diffuse  = new glm::vec3(0.18275f, 0.17f, 0.22525f);
		specular = new glm::vec3(0.332741f, 0.328643f, 0.346435f);
		shininess = 0.3f;
		break;
	case Material::Pearl:
		ambient  = new glm::vec3(0.25f, 0.20725f, 0.20725f);
		diffuse  = new glm::vec3(1.0f, 0.829f, 0.829f);
		specular = new glm::vec3(0.296648f, 0.296648f, 0.296648f);
		shininess = 0.088f;
		break;
	case Material::Ruby:
		ambient  = new glm::vec3(0.1745f, 0.01175f, 0.01175f);
		diffuse  = new glm::vec3(0.61424f, 0.04136f, 0.04136f);
		specular = new glm::vec3(0.727811f, 0.626959f, 0.626959f);
		shininess = 0.6f;
		break;
	case Material::Turquoise:
		ambient  = new glm::vec3(0.1f, 0.18725f, 0.1745f);
		diffuse  = new glm::vec3(0.396f, 0.74151f, 0.69102f);
		specular = new glm::vec3(0.297254f, 0.30829f, 0.306678f);
		shininess = 0.1f;
		break;
	case Material::Brass:
		ambient  = new glm::vec3(0.329412f, 0.223529f, 0.027451f);
		diffuse  = new glm::vec3(0.780392f, 0.568627f, 0.113725f);
		specular = new glm::vec3(0.992157f, 0.941176f, 0.807843f);
		shininess = 0.21794872f;
		break;
	case Material::Bronze:
		ambient  = new glm::vec3(0.2125f, 0.1275f, 0.054f);
		diffuse  = new glm::vec3(0.714f, 0.4284f, 0.18144f);
		specular = new glm::vec3(0.393548f, 0.271906f, 0.166721f);
		shininess = 0.2f;
		break;
	default:
		ambient   = new glm::vec3(1.0f, 1.0f, 1.0f);
		diffuse   = new glm::vec3(1.0f, 1.0f, 1.0f);
		specular  = new glm::vec3(1.0f, 1.0f, 1.0f);
		shininess = 1.0f;
		break;
	}
}

Cube::Cube(const Cube& cube) :
	shininess(cube.shininess)
{
	std::cout << "Cube was created by copy constructor\n";
	position = new glm::vec3(cube.position->x, cube.position->y, cube.position->z);
	ambient  = new glm::vec3(cube.ambient->x,  cube.ambient->y,  cube.ambient->z);
	diffuse  = new glm::vec3(cube.diffuse->x,  cube.diffuse->y,  cube.diffuse->z);
	specular = new glm::vec3(cube.specular->x, cube.specular->y, cube.specular->z);
}

Cube::~Cube()
{
	std::cout << "Cube was deleted by destructor\n";
	delete[] ambient;
	delete[] diffuse;
	delete[] specular;
	delete[] position;
}

void Cube::setupShader(const Shader& shader)
{
	shader.setVec3("material.ambient", *ambient);
	shader.setVec3("material.diffuse", *diffuse);
	shader.setVec3("material.specular", *specular);
	shader.setFloat("material.shininess", shininess);
	shader.setVec3("light.ambient",  1.0f, 1.0f, 1.0f);
	shader.setVec3("light.diffuse",  1.0f, 1.0f, 1.0f);
	shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
}

float* Cube::getVertArray()
{
	return vertices;
}

void Cube::setPosition(const glm::vec3& position)
{
	this->position = new glm::vec3{};
	this->position->x = position.x;
	this->position->y = position.y;
	this->position->z = position.z;
}

glm::vec3 Cube::getPosition()
{
	return *position;
}

int Cube::getVertCount()
{
	return vertCount;
}

int Cube::getVertArraySize()
{
	return 4 * vertCount;
}

float Cube::vertices[216] = 
{
					  //normal vectors coords
-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};