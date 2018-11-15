#pragma once
#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

namespace Model
{
	struct Vertex
	{
		glm::vec4 useless;
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 textureCoords;
	};
}

#endif