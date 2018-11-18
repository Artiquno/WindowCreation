#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Camera
{
	class Camera
	{
	private:
		glm::mat4 projectionMatrix;

		glm::vec3 position;
		glm::vec3 direction;

		float pitch;
		float yaw;

		unsigned int width;
		unsigned int height;

		float fov;

		float near;
		float far;

	public:
		Camera(unsigned int width, unsigned int height, float fov = 45.0f, float near = 0.1f, float far = 100.0f);
		~Camera();

		void setDimensions(unsigned int width, unsigned int height);

		void translate(const glm::vec3& vector);
		void rotate(float pitch, float yaw);
		void setFov(float fov);


		glm::mat4 getViewMatrix() { return glm::lookAt(position, position + direction, glm::vec3(0.0f, 1.0f, 0.0f)); }
		glm::mat4 getProjectionMatrix() { return projectionMatrix; }

	private:
		void updateProjectionMatrix();
	};
}

#endif // !CAMERA_H
