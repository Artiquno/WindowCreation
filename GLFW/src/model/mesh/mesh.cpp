#include "mesh.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Model
{
	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) :
		verts(vertices), indices(indices)
	{
		// Initialize VAO
		glGenVertexArrays(1, &vao);

		// Initialize VBOs
		glGenBuffers(1, &vertexVbo);
		glGenBuffers(1, &colorVbo);
		glGenBuffers(1, &uvVbo);

		// Initialize EBO
		glGenBuffers(1, &ebo);

		// Use this vao
		// I think the next two buffers get bound to this vao
		// I should make sure whether this is what this means
		glBindVertexArray(vao);

		// Pass data to vbo
		glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Model::Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

		// Assign attribute pointers
		int posOffset = (int)(&vertices[0].position) - (int)(vertices.data());
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Model::Vertex), (void*)(posOffset));
		glEnableVertexAttribArray(0);

		int colorOffset = (int)(&vertices[0].color) - (int)(vertices.data());
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Model::Vertex), (void*)(colorOffset));
		glEnableVertexAttribArray(1);

		int uvOffset = (int)(&vertices[0].textureCoords) - (int)(vertices.data());
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Model::Vertex), (void*)(uvOffset));
		glEnableVertexAttribArray(2);

		// Pass data to ebo
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	}
	Mesh::~Mesh()
	{

	}
	void Mesh::draw(GLenum drawMode, unsigned int tex1, unsigned int tex2, Shader::Program program, const glm::mat4& transform)
	{
		glBindVertexArray(vao);

		program.setMatrix4f("model", 1, GL_FALSE, transform);

		// Setup shader here or something

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex2);

		// Draw
		glDrawElements(drawMode, indices.size() * sizeof(unsigned int), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}
