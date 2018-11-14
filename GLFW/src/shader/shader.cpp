#include "shader.h"

#include "../definitions.h"

#include <string>
#include <fstream>
#include <iostream>

namespace Shader
{
	Shader::Shader(const std::string& path, GLenum type) :
		path(path), type(type)
	{
		this->id = glCreateShader(type);

		std::string s = loadSource();
		compile(s);
		// delete source;	// I think this is deleted when destroying s
	}

	Shader::~Shader()
	{
		glDeleteShader(id);
	}

	std::string Shader::loadSource()
	{
		std::ifstream file(path);
		if (!file.is_open())
		{
			std::cerr << "Could not read file " << path << std::endl;
			return std::string();
		}

		std::string source( (std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
		file.close();

		return source;
	}

	void Shader::compile(const std::string& source)
	{
		const char* s = source.c_str();
		int size = source.size();
		glShaderSource(id, 1, &s, &size);

		glCompileShader(id);

		int success;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char error[512];
			glGetShaderInfoLog(id, sizeof(error), NULL, error);
			std::cout << "Failed to compile shader '" << path << "':"
				<< std::endl << error;
			exit(ExitStatus::ShaderCompileError);
		}
	}
}