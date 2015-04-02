
#version 400

layout(location=0) in vec3 in_Position;
layout(location=1) in vec2 in_TexCoord;

out vec2 vs_outTexCoord;

void main()
{
	gl_Position = vec4(in_Position, 1.0f);
	vs_outTexCoord = in_TexCoord;
}