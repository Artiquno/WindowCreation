#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aCol;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 col_out;
out vec2 texCoord;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	gl_PointSize = 10.0;
	col_out = aCol;
	texCoord = aTexCoord;
}