#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>

#include <glm/glm.hpp>

namespace Shader
{
	class Shader
	{
	protected:
		GLenum type;		// Vertex or Fragment? Or something else?
		std::string path;	// Path to the shader's source
		GLuint id;			// Id returned by OpenGl
	public:
		Shader(const std::string& path, GLenum type);
		virtual ~Shader();

		GLuint getId() const { return id; }
		const std::string& getPath() const { return path; }
	protected:
		std::string loadSource();
		void compile(const std::string& source);
	};
}

#endif