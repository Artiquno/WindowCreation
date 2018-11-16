#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Camera
{
	Camera::Camera(unsigned int width, unsigned int height, float fov, float near, float far) :
		width(width), height(height), fov(fov), near(near), far(far)	// Wherever you are
	{
		viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -6.0f));
		viewMatrix = glm::rotate(viewMatrix, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		viewMatrix = glm::rotate(viewMatrix, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));

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