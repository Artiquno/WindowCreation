#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

namespace Camera
{
	class Camera
	{
	private:
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;

		unsigned int width;
		unsigned int height;

		float fov;

		float near;
		float far;

	public:
		Camera(unsigned int width, unsigned int height, float fov = 45.0f, float near = 0.1f, float far = 100.0f);
		~Camera();

		void setDimensions(unsigned int width, unsigned int height);
		void setFov(float fov);

		glm::mat4& getViewMatrix() { return viewMatrix; }

	private:
		void updateProjectionMatrix();
	};
}

#endif // !CAMERA_H
