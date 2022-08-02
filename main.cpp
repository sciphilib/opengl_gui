#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>
#include <vector>

#include "stb_image.h"	
#include "Shader.h"
#include "Camera.h"
#include "Cube.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xpos, double ypos);



const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


// Camera object
Camera camera;
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f; // time of last frame

float lastX = SCR_WIDTH / 2.0;
float lastY = SCR_HEIGHT / 2.0;


// the light source's location in world-space
glm::vec3 lightPos(2.0f, 1.0f, 1.0f);

//
//float Cube::vertices[] =
//{
//					  //normal vectors coords
//-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
// 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
// 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
// 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//
//-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
// 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
// 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
// 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//
//-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//
// 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
// 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
// 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
// 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
// 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
// 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//
//-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
// 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
// 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
// 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//
//-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
// 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
// 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
// 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
//};

int main() 
{

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(800, 600, "Title", NULL, NULL);
	if (window == NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	// tell GLFW call callback function on every window resize.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, lastX, lastY);


	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
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

	// (VBO). Generate vertex buffer object.
	unsigned int VBO;

	// (VAO). Generate vertex array object.
	unsigned int objectVAO;

	// Cube object
	glGenVertexArrays(1, &objectVAO);
	glGenBuffers(1, &VBO);
	
	// Hook this buf obj and tell what it will store.
	// vertex array data -> GL_ARRAY_BUFFER.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Cube::getVertArraySize(), Cube::getVertArray(), GL_STATIC_DRAW);
	// position attributes
	glBindVertexArray(objectVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// second, configure the light's VAO 
	// (VBO stays the same the vertices
	// are the same for the light object which is also a 3D cube)
	unsigned int sunVAO;
	glGenVertexArrays(1, &sunVAO);
	glBindVertexArray(sunVAO);

	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	Shader objectShader("VertexObjectShader.glsl", "FragmentObjectShader.glsl");
	Shader sunShader("VertexSunShader.glsl", "FragmentSunShader.glsl");
	//objectShader.use();
	//objectShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
	//objectShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f); 
	//objectShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	//objectShader.setFloat("material.shininess", 32.0f);
	//objectShader.setVec3("light.position", lightPos);
	//objectShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	//objectShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
	//objectShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

	//glm::vec3 lightColor{}; // !!!!!!!!!!
	//lightColor.x = std::sin(glfwGetTime() * 2.0f);
	//lightColor.y = std::sin(glfwGetTime() * 0.7f);
	//lightColor.z = std::sin(glfwGetTime() * 1.3f);

	Cube* emerald_cube = new Cube(Material::Emerald);
	Cube* jade_cube = new Cube(Material::Jade);
	Cube* obsidian_cube = new Cube(Material::Obsidian);
	Cube* pearl_cube = new Cube(Material::Pearl);
	Cube* ruby_cube = new Cube(Material::Ruby);
	Cube* turquoise_cube = new Cube(Material::Turquoise);
	Cube* brass_cube = new Cube(Material::Brass);
	Cube* bronze_cube = new Cube(Material::Bronze);
	pearl_cube->setPosition(glm::vec3(-1.0f, 0.0f, -1.0f));
	emerald_cube->setPosition(glm::vec3(1.0f, 0.0f, -1.0f));
	jade_cube->setPosition(glm::vec3(3.0f, 0.0f, -1.0f));
	obsidian_cube->setPosition(glm::vec3(5.0f, 0.0f, -1.0f));
	ruby_cube->setPosition(glm::vec3(-1.0f, 2.0f, -1.0f));
	turquoise_cube->setPosition(glm::vec3(1.0f, 2.0f, -1.0f));
	brass_cube->setPosition(glm::vec3(3.0f, 2.0f, -1.0f));
	bronze_cube->setPosition(glm::vec3(5.0f, 2.0f, -1.0f));

	std::vector<Cube*> cubes;
	//cubes.reserve(20);
	cubes.push_back(emerald_cube);
	cubes.push_back(jade_cube);
	cubes.push_back(obsidian_cube);
	cubes.push_back(pearl_cube);
	cubes.push_back(ruby_cube);
	cubes.push_back(turquoise_cube);
	cubes.push_back(brass_cube);
	cubes.push_back(bronze_cube);


	// camera/view transformation
	glm::vec3 cameraPosition(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
	camera = Camera(cameraPosition, cameraFront, cameraUp);

	// Render loop
	while (!glfwWindowShouldClose(window)) 
	{
		// per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		// user input
		// ----------
		processInput(window);

		// render
		// ------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// -------------
		// Object render
		// -------------

		glm::mat4 model;
		glm::mat4 normalModel;
		glm::mat4 view;
		glm::mat4 projection;

		for (auto cube : cubes)
		{
			objectShader.use();
			cube->setupShader(objectShader);

			// model matrix and model matrix for normal vectors
			model = glm::mat4(1.0f);
			normalModel = glm::mat4(1.0f);
			model = glm::translate(model, cube->getPosition());
			normalModel = glm::transpose(glm::inverse(model));
			objectShader.setVec3("light.position", lightPos);
			objectShader.setMat4("model", model);
			objectShader.setMat4("normalModel", normalModel);
			// view matrix
			view = camera.lookAt();
			objectShader.setMat4("view", view);
			objectShader.setVec3("viewPos", camera.position);
			// projection matrix
			projection = glm::perspective(glm::radians(camera.getCameraFov()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			objectShader.setMat4("projection", projection);
			glBindVertexArray(objectVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);


		}

		
		
		
		// -------------
		// Sun render
		// -------------
		
		// model matrix
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		sunShader.use();
		sunShader.setMat4("model", model);
		// the same view matrix
		sunShader.setMat4("view", view);
		// the same projection matrix
		sunShader.setMat4("projection", projection);
		glBindVertexArray(sunVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// -------------

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window); // swap the color buffer where final output image was drawn.
		glfwPollEvents(); // ñhecks if any events are triggered (keyboard output, etc).
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &objectVAO);
	glDeleteVertexArrays(1, &sunVAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
	std::cout << "Deleting\n";
	for (auto cube : cubes)
		delete cube;
	//emerald_cube->~Cube();
	//jade_cube->~Cube();
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.processKeyboard(Camera_Movement::forward, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.processKeyboard(Camera_Movement::backward, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.processKeyboard(Camera_Movement::left, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.processKeyboard(Camera_Movement::right, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	static bool lock = 0, wf_mode = 0;
	if (!glfwGetKey(window, GLFW_KEY_Q))
		lock = 0;
	if (glfwGetKey(window, GLFW_KEY_Q) && lock == 0)
	{
		glPolygonMode(GL_FRONT_AND_BACK, (wf_mode = 1 - wf_mode) ? GL_LINE : GL_FILL);
		lock = 1;
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: listen to mouse-movement events
// -------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	// x = 0 and y = 0 in upper-left
	// depends on basis 
	float xoffset = (xpos - lastX);
	float yoffset = (ypos - lastY);
	lastX = xpos;
	lastY = ypos;

	camera.processMouseMovement(xoffset, yoffset);
}

// glfw: listen to mouse-scroll events
// -----------------------------------
void scroll_callback(GLFWwindow* window, double xpos, double ypos)
{
	// A normal mouse wheel, being vertical, provides offsets along the Y - axis.
	camera.processMouseScroll(static_cast<float>(ypos));
}