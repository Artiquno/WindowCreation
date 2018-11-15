#pragma once
#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "vertex_shader.h"
#include "fragment_shader.h"

#include <GL/glew.h>
#include <string>
#include <unordered_map>

namespace Shader
{
	class Program
	{
	protected:
		VertexShader vertexShader;
		FragmentShader fragmentShader;
		GLuint id;

		// Uniform locations of the program
		std::unordered_map<std::string, GLint> uniforms;
	public:
		// Create and compile
		Program(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
		~Program();

		void use();

		GLuint getId() const { return id; }
		VertexShader& getVertexShader() { return vertexShader; }
		const VertexShader getVertexShader() const { return vertexShader; }

		FragmentShader& getfragmentShader(){ return fragmentShader; }
		const FragmentShader getFragmentShader() const { return fragmentShader; }

		// Change the vertex shader and update
		void setVertexShader(std::string path);
		// Change the fragment shader and update
		void setFragmentShader(std::string path);

		void setInt(const std::string& name, int value);
		// Set a float 3-vector uniform
		void setFloat3(const std::string& name, GLsizei count, const float *value);
		// Set a 4x4 matrix
		void setMatrix4f(const std::string& name, GLsizei count, GLboolean transpose, const glm::mat4& value);
	protected:
		// Compile shaders and link program
		void link();
		GLint getUniformLocation(const std::string& name);
	};
}

#endif