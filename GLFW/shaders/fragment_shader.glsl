#version 330 core
out vec4 FragColor;

in vec4 col_out;
in vec2 texCoord;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{
	FragColor = mix(texture(tex1, texCoord), texture(tex2, texCoord), 0.3) * col_out;
}