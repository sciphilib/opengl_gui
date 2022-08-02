#include "Camera.h"

Camera::Camera(const glm::vec3& cameraPosition, const glm::vec3& cameraTarget, const glm::vec3& cameraUp)
{
	init(cameraPosition, cameraTarget, cameraUp);
}

glm::mat4 Camera::lookAt()
{
	
	// create translation matrix
	glm::mat4 mTran(1.0f);
	mTran = glm::translate(mTran, -position);
	// create rotation matrix
	glm::mat4 mRot(1.0f);
	mRot[0] = glm::vec4(left, 0.0f);	
	mRot[1] = glm::vec4(up, 0.0f);
	mRot[2] = glm::vec4(forward, 0.0f);
	// suppose we need to rotate the camera to +30 degree along X-axis
	// it is equivalent to rotate the entire scene in opposite direction
	// -30 degree on X-axis, thats why we need to invert rotation matrix
	// rotation matrix is an orthogonal matrix, so the invert is just a transpose
	mRot = glm::transpose(mRot);
	
	// return view matrix
	// Mview = mRot * mTran
	return mRot * mTran;
}

void Camera::init(const glm::vec3& cameraPosition, const glm::vec3& cameraTarget, const glm::vec3& cameraUp)
{
	position = cameraPosition;
	// Compute direction (forward vector, -z)
	forward = glm::normalize(cameraPosition - cameraTarget);
	// Compute left axis (left vextor, -x)
	left = glm::normalize(glm::cross(cameraUp, forward));
	// Compute up axis (y)
	up = glm::normalize(glm::cross(forward, left));
	// set default camera attributes
	yaw = 90.0f;
	pitch = 0.0f;
	zoom = 45.0f;
	movementSpeed		= 2.5f;
	mouseSensetivity	= 0.1f;
}

void Camera::processKeyboard(Camera_Movement movement, float deltaTime)
{
	float velocity = movementSpeed * deltaTime;
	if (movement == Camera_Movement::forward)
		position -= forward * velocity;
	if (movement == Camera_Movement::backward)
		position += forward * velocity;
	if (movement == Camera_Movement::left)
		position -= left * velocity;	
	if (movement == Camera_Movement::right)
		position += left * velocity;

	//positionVector.y = 0.0f; // this one-liner keeps the user at the ground level (xz plane)
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	yaw		+= mouseSensetivity * xoffset;
	pitch	+= mouseSensetivity * yoffset;
	
	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}
	updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
	zoom -= yoffset;
	if (zoom < 1.0f)
		zoom = 1.0f;
	if (zoom > 45.0f)
		zoom = 45.0f;
}

void Camera::setEulerAngles(float yaw, float pitch)
{
	this->yaw = yaw;
	this->pitch = pitch;
}

void Camera::updateCameraVectors()
{
	forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward.y = sin(glm::radians(pitch));
	forward	= glm::normalize(forward);
	left	= glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), forward));
	up		= glm::normalize(glm::cross(forward, left));
}

float Camera::getCameraFov()
{
	return zoom;
}