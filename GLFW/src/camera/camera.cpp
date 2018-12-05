#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Camera
{
	const float Camera::rotationSensitivity = 0.1f;
	const float Camera::zoomSensitivity = 0.1f;

	Camera::Camera(unsigned int width, unsigned int height, float fov, float near, float far) :
		width(width), height(height), fov(fov),
		near(near), far(far)	// Wherever you are
	{
		position = glm::vec3(2.0f, 2.0f, 6.0f);
		//direction = glm::vec3(0.0f, 0.0f, -1.0f);

		pitch = -15.0f;
		yaw = -90.0f;
		rotate(0.0f, 0.0f);	// Setup the direction

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
		// Can't think of a general solution
		if (vector.z > 0.01f || vector.z < -0.01f)
		{
			moveForward(vector.z);
		}
		if (vector.y > 0.01f || vector.y < -0.01f)
		{
			moveVertically(vector.y);
		}
		if (vector.x > 0.01f || vector.x < -0.01f)
		{
			moveSideways(vector.x);
		}
	}

	void Camera::moveForward(float amount)
	{
		// Direction points backwards
		position += direction * (-amount);
	}
	void Camera::moveVertically(float amount)
	{
		glm::vec3 globalUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 localX = glm::normalize(glm::cross(direction, globalUp));
		glm::vec3 localY = glm::normalize(glm::cross(localX, direction));
		position += localY * amount;
	}
	void Camera::moveSideways(float amount)
	{
		glm::vec3 globalUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 localX = glm::normalize(glm::cross(direction, globalUp));
		position += localX * amount;
	}

	void Camera::rotate(float pitch, float yaw)
	{
		this->pitch += pitch;
		this->yaw += yaw;

		if (this->pitch > 89.0f)
		{
			this->pitch = 89.0f;
		}
		if (this->pitch < -89.0f)
		{
			this->pitch = -89.0f;
		}

		direction.x = cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw));
		direction.y = sin(glm::radians(this->pitch));
		direction.z = cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw));
	}

	void Camera::setFov(float fov)
	{ 
		if (fov <= 1.0f)
			fov = 1.0f;
		if (fov >= 45.0f)
			fov = 45.0f;

		this->fov = fov;
		updateProjectionMatrix();
	}

	void Camera::updateProjectionMatrix()
	{
		projectionMatrix = glm::perspective(glm::radians(fov), width / (float)height, near, far);
	}
}