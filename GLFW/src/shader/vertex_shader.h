#pragma once
#ifndef VERTEX_SHADER_H

#include "shader.h"

#include <string>
namespace Shader
{
	class VertexShader : public Shader
	{
	public:
		VertexShader(const std::string& path) : Shader(path, GL_VERTEX_SHADER) {}
		~VertexShader() {}
	};
}
#endif // !VERTEX_SHADER_H
