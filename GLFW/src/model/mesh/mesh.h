#pragma once
#ifndef MESH_H
#define MESH_H

#include "vertex.h"
#include "../../shader/shader_program.h"

#include <GL/glew.h>

#include <vector>
#include <glm/glm.hpp>

namespace Model
{
	class Mesh
	{
	private:
		std::vector<Vertex> verts;
		std::vector<unsigned int> indices;

		GLuint vao;
		GLuint vertexVbo;
		GLuint colorVbo;
		GLuint uvVbo;
		GLuint ebo;

	public:
		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		~Mesh();

		// Params are temporary until i make the model/object class
		void draw(GLenum drawMode, unsigned int tex1, unsigned int tex2, Shader::Program program, const glm::mat4& transform);
	};
}

#endif // !MESH_H
