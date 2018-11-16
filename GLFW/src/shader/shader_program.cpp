#include "shader_program.h"

#include "vertex_shader.h"
#include "fragment_shader.h"
#include "../definitions.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace Shader
{
	Program::Program(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
		: vertexShader(VertexShader(vertexShaderPath)), fragmentShader(FragmentShader(fragmentShaderPath))
	{
		id = glCreateProgram();
		link();
	}
	Program::~Program()
	{
		glDeleteProgram(id);
	}
	void Program::use()
	{
		glUseProgram(id);
	}
	void Program::setVertexShader(std::string path)
	{
		vertexShader = VertexShader(path);
	}

	void Program::setFragmentShader(std::string path)
	{
		fragmentShader = FragmentShader(path);
	}

	void Program::setInt(const std::string& name, int value)
	{
		use();
		glUniform1i(getUniformLocation(name), value);
		glUseProgram(0);
	}

	void Program::setFloat3(const std::string & name, GLsizei count, const float *value)
	{
		use();
		glUniform3fv(getUniformLocation(name), count, value);
		glUseProgram(0);
	}

	void Program::setMatrix4f(const std::string & name, GLsizei count, GLboolean transpose, const glm::mat4 & value)
	{
		use();
		glUniformMatrix4fv(getUniformLocation(name), 1, transpose, glm::value_ptr(value));
		glUseProgram(0);
	}

	void Program::link()
	{
		glAttachShader(id, vertexShader.getId());
		glAttachShader(id, fragmentShader.getId());
		glLinkProgram(id);

		// Check for errors
		int success;
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success)
		{
			char error[512];
			glGetProgramInfoLog(id, sizeof(error), NULL, error);
			std::cerr << "Failed to link shader program:" << std::endl << error << std::endl;
			exit(ExitStatus::ShaderProgramLinkError);
		}
	}

	GLint Program::getUniformLocation(const std::string & name)
	{
		if (uniforms.find(name) == uniforms.end())
		{
			uniforms[name] = glGetUniformLocation(id, name.c_str());
		}
		return uniforms[name];
	}
}