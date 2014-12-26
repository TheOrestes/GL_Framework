
#version 400

layout(location=0) in vec3 position;
layout(location=1) in vec4 inColor;

uniform mat4 matWVP;

out vec4 color;

void main()
{
	color = inColor;
	gl_Position = matWVP * vec4(position, 1.0);
}