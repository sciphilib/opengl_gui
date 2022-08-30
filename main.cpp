#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

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
std::vector<glm::vec3> generateGrid(float size, float step);
void ImGuiStart(GLFWwindow* window, const Shader& shader);

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;


// camera object
Camera camera;
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f; // time of last frame

float lastX = SCR_WIDTH / 2.0;
float lastY = SCR_HEIGHT / 2.0;

// the light source's location in world-space
glm::vec3 lightPos(2.0f, 3.0f, 1.0f);

// menu mode
bool isMenu = false;

// dynamic array of cube objects
std::vector<Cube*> cubes;


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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Title", NULL, NULL);
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


	// ---------------------------------------
	// Setup Dear ImGui context
	// ---------------------------------------
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	const char* glsl_version = "#version 130";
	ImGui_ImplOpenGL3_Init(glsl_version);

	// ---------------------------------------

	// Grid
	const std::vector<glm::vec3>& grid = generateGrid(30, 5);
	unsigned int gridVBO, gridVAO;
	glGenVertexArrays(1, &gridVAO);
	glGenBuffers(1, &gridVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
	glBufferData(GL_ARRAY_BUFFER, grid.size() * sizeof(glm::vec3), grid.data(), GL_STATIC_DRAW);
	glBindVertexArray(gridVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);


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

	// the light's VAO 
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
	Shader gridShader("VertexGridShader.glsl", "FragmentGridShader.glsl");

	// camera/view transformation
	glm::vec3 cameraPosition(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
	camera = Camera(cameraPosition, cameraFront, cameraUp);

	// Render loop
	glm::mat4 model;
	glm::mat4 normalModel;
	glm::mat4 view;
	glm::mat4 projection;
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

		objectShader.use();
		model = glm::mat4(1.0f);
		objectShader.setMat4("model", model);
		// view matrix
		view = camera.lookAt();
		objectShader.setMat4("view", view);
		objectShader.setVec3("viewPos", camera.position);
		// projection matrix
		projection = glm::perspective(glm::radians(camera.getCameraFov()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		objectShader.setMat4("projection", projection);

		//todo increase program fps and figure out why its low (using profiler)
		for (auto cube : cubes)
		{
			cube->setupShader(objectShader);

			// model matrix and model matrix for normal vectors
			model = glm::mat4(1.0f);
			normalModel = glm::mat4(1.0f);
			model = glm::translate(model, cube->getPosition());
			normalModel = glm::transpose(glm::inverse(model));
			objectShader.setVec3("light.position", lightPos);
			objectShader.setMat4("model", model);
			objectShader.setMat4("normalModel", normalModel);
			glBindVertexArray(objectVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}

		
		// -------------
		// Sun render
		// -------------
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
		// Grid render
		// ------------- 
		gridShader.use();
		model = glm::mat4(1.0f);
		gridShader.setMat4("model", model);
		gridShader.setMat4("view", view);
		gridShader.setMat4("projection", projection);
		glBindVertexArray(gridVAO);
		glDrawArrays(GL_LINES, 0, grid.size());
		glBindVertexArray(0);

		if (isMenu)
			ImGuiStart(window, objectShader);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwPollEvents(); // ñhecks if any events are triggered (keyboard output, etc).
		glfwSwapBuffers(window); // swap the color buffer where final output image was drawn.
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
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

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

	// switch on/off imgui widgets
	static bool menuLock = 0;
	if (!glfwGetKey(window, GLFW_KEY_SPACE))
		menuLock = 0;
	if (glfwGetKey(window, GLFW_KEY_SPACE) && !menuLock)
	{
		isMenu = !isMenu;
		if (isMenu)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else 
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetCursorPos(window, lastX, lastY);
		}
		menuLock = 1;
	}

	static bool lock = 0, wf_mode = 0;
	if (!glfwGetKey(window, GLFW_KEY_Q))
		lock = 0;
	if (glfwGetKey(window, GLFW_KEY_Q) && lock == 0)
	{
		glPolygonMode(GL_FRONT_AND_BACK, (wf_mode = 1 - wf_mode) ? GL_LINE : GL_FILL);
		lock = 1;
	}


	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
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
	if (!isMenu)
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

}

// glfw: listen to mouse-scroll events
// -----------------------------------
void scroll_callback(GLFWwindow* window, double xpos, double ypos)
{
	// A normal mouse wheel, being vertical, provides offsets along the Y - axis.
	camera.processMouseScroll(static_cast<float>(ypos));
}


std::vector<glm::vec3> generateGrid(float size, float step)
{
	std::vector<glm::vec3> points;

	for (float i = step; i <= size; i += step)
	{
		//// lines parallel to Z-axis
		points.push_back(glm::vec3( i, 0.0f,  size));
		points.push_back(glm::vec3( i, 0.0f, -size));
		points.push_back(glm::vec3(-i, 0.0f,  size));
		points.push_back(glm::vec3(-i, 0.0f, -size));

		//// lines parallel to X-axis
		points.push_back(glm::vec3( size, 0.0f,  i));
		points.push_back(glm::vec3(-size, 0.0f,  i));
		points.push_back(glm::vec3( size, 0.0f, -i));
		points.push_back(glm::vec3(-size, 0.0f, -i));
	}

	// X-axis
	points.push_back(glm::vec3( size, 0.0f, 0.0f));
	points.push_back(glm::vec3(-size, 0.0f, 0.0f));
	// Z-axis
	points.push_back(glm::vec3(0.0f, 0.0f,  size));
	points.push_back(glm::vec3(0.0f, 0.0f, -size));
	
	return points;
}


void ImGuiStart(GLFWwindow* window, const Shader& shader)
{
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	static float f = 0.0f;
	static int counter = 0;

	//ImGui::ShowDemoWindow();
	ImGui::Begin("Scene settings");
	ImGui::Text("Cube creation");
	
	static int item_current = 0;
	const char* items[] = { "Emerald", "Jade", "Obsidian", "Pearl", "Ruby", "Turquoise", "Brass", "Bronze" };
	ImGui::Combo("Cube material", &item_current, items, IM_ARRAYSIZE(items));

	static float vec3a[3] = { 0.0f, 0.0f, 0.0f};
	ImGui::InputFloat3("Cube position", vec3a);

	// todo update array's size after overflow (now size = 20)
	// todo deleting objects
	// todo change focused object's position, size, etc...
	static const char* objectList[20];
	if (ImGui::Button("Create cube"))  
	{
		glm::vec3 position(vec3a[0], vec3a[1], vec3a[2]);
		switch (item_current)
		{
		case 0:
			cubes.push_back(new Cube(Material::Emerald, position));
			break;
		case 1:
			cubes.push_back(new Cube(Material::Jade, position));
			break;
		case 2:
			cubes.push_back(new Cube(Material::Obsidian, position));
			break;
		case 3:
			cubes.push_back(new Cube(Material::Pearl, position));
			break;
		case 4:
			cubes.push_back(new Cube(Material::Ruby, position));
			break;
		case 5:
			cubes.push_back(new Cube(Material::Turquoise, position));
			break;
		case 6:
			cubes.push_back(new Cube(Material::Brass, position));
			break;
		case 7:
			cubes.push_back(new Cube(Material::Bronze, position));
			break;
		default:
			break;
		}
		objectList[Cube::getIDs() - 1] = cubes.at(static_cast<std::vector<Cube*, std::allocator<Cube*>>::size_type>(Cube::getIDs()) - 1)->getLabel().c_str();
	}

	ImGui::Text("Light position:");
	static float lightPosition_x = lightPos.x;
	static float lightPosition_y = lightPos.y;
	static float lightPosition_z = lightPos.z;
	ImGui::DragFloat("X", &lightPos.x, 0.05f);
	ImGui::DragFloat("Y", &lightPos.y, 0.05f);
	ImGui::DragFloat("Z", &lightPos.z, 0.05f);

	static bool showObjectsWindow = false;
	ImGui::Checkbox("Show all objects.", &showObjectsWindow);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
	
	// show window that displays a list of objects
	if (showObjectsWindow)
	{
		// pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Begin("Objects list", &showObjectsWindow);   
		static int currentObject = 0;
		if (ImGui::BeginListBox("Objects names", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (int n = 0; n < Cube::getIDs(); n++)
			{
				const bool is_selected = (currentObject == n);
				if (ImGui::Selectable(objectList[n], is_selected))
					currentObject = n;

				if (n == currentObject)
					cubes.at(currentObject)->setIsFocused(true);
				else
					cubes.at(n)->setIsFocused(false);
			}
			ImGui::EndListBox();
		}

		if (ImGui::Button("Close"))
			showObjectsWindow = false;
		ImGui::End();
	}

	
	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}