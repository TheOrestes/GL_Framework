
#version 400

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec2 in_Tex;

// uniforms...
uniform mat4 matWorld;
uniform mat4 matView;
uniform mat4 matProj;

void main()
{	
	mat4 WVP = matProj * matView * matWorld;

	gl_Position = WVP * vec4(in_Position, 1.0);
}