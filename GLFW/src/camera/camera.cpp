#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Camera
{
	Camera::Camera(unsigned int width, unsigned int height, float fov, float near, float far) :
		width(width), height(height), fov(fov),
		near(near), far(far)	// Wherever you are
	{
		// Moving the camera 6.0f towards +Z
		// The best way to move a spaceship is
		// by moving the whole universe around it instead

		position = glm::vec3(2.0f, 1.0f, 6.0f);
		direction = glm::vec3(0.0f, 0.0f, -1.0f);

		pitch = 0.0f;
		yaw = 0.0f;
		/*viewMatrix = glm::rotate(viewMatrix, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		viewMatrix = glm::rotate(viewMatrix, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));*/

		updateProjectionMatrix();
	}

	Camera::~Camera()
	{

	}

	void Camera::setDimensions(unsigned int width, unsigned int height)
	{
		this->width = width;
		this->height = height;
		updateProjectionMatrix();
	}

	void Camera::translate(const glm::vec3& vector)
	{
		position += glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void Camera::rotate(float pitch, float yaw)
	{
		this->pitch += pitch;
		this->yaw += yaw;

		direction.x = cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw));
		direction.y = sin(glm::radians(this->pitch));
		direction.z = cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw));
	}

	void Camera::setFov(float fov)
	{
		this->fov = fov;
		updateProjectionMatrix();
	}

	void Camera::updateProjectionMatrix()
	{
		projectionMatrix = glm::perspective(fov, width / (float)height, near, far);
	}
}