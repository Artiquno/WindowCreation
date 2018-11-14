#pragma once
#ifndef FRAGMENT_SHADER_H

#include "shader.h"

#include <string>
namespace Shader
{
	class FragmentShader : public Shader
	{
	public:
		FragmentShader(const std::string& path) : Shader(path, GL_FRAGMENT_SHADER) {}
		~FragmentShader() {}
	};
}
#endif // !FRAGMENT_SHADER_H
