
#version 400

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;

uniform mat4 matWorld;
uniform mat4 matView;
uniform mat4 matProj;

//out vec3 outPosition;
out vec3 Normal;

void main()
{
	mat4 WVP = matProj * matView * matWorld;
	gl_Position = WVP * vec4(in_Position, 1.0);

	//outPosition = matWorld * vec4(in_Position, 1.0);
	Normal = normalize(matWorld * vec4(in_Normal, 1.0)).xyz;
}