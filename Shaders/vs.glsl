
#version 400

layout(location=0) in vec3 in_Position;
layout(location=1) in vec4 in_Color;

out vec3 vs_outPosition;
out vec4 color;

// uniforms...
uniform mat4 matWorld;
uniform mat4 matView;
uniform mat4 matProj;

void main()
{
	vs_outPosition = (matWorld * vec4(in_Position, 1)).xyz;
	color = in_Color;

	mat4 WVP = matProj * matView * matWorld;
	gl_Position = WVP * vec4(in_Position, 1.0);
}