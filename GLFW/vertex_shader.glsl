#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 transform;
uniform vec3 color;

out vec4 col_out;
out vec2 texCoord;

void main()
{
	gl_Position = transform * vec4(aPos, 1.0);
	gl_PointSize = 10.0;
	col_out = vec4(mix(aCol, color, 0.5), 1.0);
	texCoord = aTexCoord;
}