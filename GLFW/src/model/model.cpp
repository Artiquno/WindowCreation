#include "model.h"

#include "../utils/image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Model
{
	Model::Model(const std::string& name, const Mesh& mesh, const Shader::Program& material) :
		name(name), mesh(mesh), material(material),
		position(glm::vec3(0.0f)), rotation(glm::vec3(0.0f)), _scale(glm::vec3(1.0f))
	{

	}
	Model::~Model()
	{
		for (GLuint texture : textures)
		{
			glDeleteTextures(1, &texture);
		}
	}

	void Model::translate(const glm::vec3& vector)
	{
		position += vector;
	}

	void Model::rotate(float angle, const glm::vec3 & axis)
	{
		rotation += angle * glm::normalize(axis);
	}

	void Model::scale(const glm::vec3 & scale)
	{
		_scale = scale;
	}
	void Model::scale(float scale)
	{
		_scale *= scale;
	}

	void Model::addTexture(const std::string& path)
	{
		// Load texture
		Utils::Image image(path);
		unsigned short *pixels = image.getPixels();

		GLuint texture;
		glGenTextures(1, &texture);

		glBindTexture(GL_TEXTURE_2D, texture);

		textures.push_back(texture);

		// Set parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Default is repeat
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Set texture data and create mipmaps
		GLint internalFormat = image.getBpp() == 4 ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image.getWidth(), image.getHeight(), 0, GL_RGB, GL_UNSIGNED_SHORT, pixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Texture is free! Master gave pointer to Texture!
		//stbi_image_free(pixels);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void Model::drawRaw(GLenum drawMode)
	{
		glm::mat4 model;
		model = glm::translate(model, position);
		model = glm::rotate(model, glm::length(rotation), glm::normalize(rotation));
		model = glm::scale(model, _scale);
		material.setMatrix4f("model", 1, GL_FALSE, model);
		glBindVertexArray(mesh.getVao());

		material.use();
		// Assign textures
		for (int i = 0; i < textures.size(); ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textures[i]);
		}

		mesh.draw(drawMode);

		// Clear textures
		for (int i = 0; i < textures.size(); ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		glUseProgram(0);
	}
	void Model::draw()
	{
		drawRaw(GL_TRIANGLES);
	}
	void Model::drawVerts()
	{
		drawRaw(GL_POINTS);
	}
}