#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class Camera_Movement {
	forward,
	backward,
	left,
	right
};

class Camera {
public:	

	Camera() = default;
	Camera(const glm::vec3& cameraPosition, const glm::vec3& cameraTarget, const glm::vec3& cameraUp = glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lookAt();
	void processKeyboard(Camera_Movement movement, float deltaTime);
	void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void processMouseScroll(float yoffset);
	void setEulerAngles(float yaw, float pitch);
	float getCameraFov();
private:
	// camera attributes
	glm::vec3 positionVector;
	glm::vec3 forwardVector;
	glm::vec3 upVector;
	glm::vec3 leftVector;

	// euler angles
	float yaw;
	float pitch;

	// camera options
	float movementSpeed;
	float mouseSensetivity;
	float zoom;

	void init(const glm::vec3& cameraPosition, const glm::vec3& cameraTarget, const glm::vec3& cameraUp);
	void updateCameraVectors();
};

