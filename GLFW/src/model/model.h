#pragma once
#ifndef MODEL_H
#define MODEL_H

#include "mesh/mesh.h"
#include "../shader/shader_program.h"

#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace Model
{
	class Model
	{
	protected:
		std::string name;
		Mesh mesh;
		Shader::Program material;
		glm::mat4 transform;
		std::vector<GLuint> textures;
	public:
		Model(const std::string& name, const Mesh& mesh, const Shader::Program& material);
		virtual ~Model();

		void translate(const glm::vec3& vector);
		// Until i learn about quaternions
		void rotate(float angle, const glm::vec3& axis);
		void scale(const glm::vec3& scale);
		// Uniform scale
		void scale(float scale);

		void addTexture(const std::string& path);
		void drawRaw(GLenum drawMode);	// Should be protected?
		void draw();
		void drawVerts();
	};
}

#endif // !MODEL_H
